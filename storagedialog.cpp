#include "storagedialog.h"
#include "ui_storagedialog.h"
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QStringList>
#include "querydialog.h"
#include "mysqlquerymodel.h"
#include "insertdialog.h"
#include "myitemdelegate.h"
#include "storagemanage.h"
#include "stock_mainwindow.h"
#include "globaldata.h"
#include "sale_widget.h"

//int is_admin = 1;

StorageDialog::StorageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StorageDialog)
{
    ui->setupUi(this);
    queryDialog = new QueryDialog(this);
    insertDialog = new InsertDialog(this);
    db = QSqlDatabase::database();
    QSqlQuery query(db);

    // 处理model
    model = new MySqlQueryModel;
    model->setQuery("select * from Main_view", db);
    if (model->lastError().isValid()) qDebug() << model->lastError().text();
    model->setHeaderData(0, Qt::Horizontal, "仓库");
    model->setHeaderData(1, Qt::Horizontal, "卖家");
    model->setHeaderData(2, Qt::Horizontal, "商品");
    model->setHeaderData(3, Qt::Horizontal, "数量");
    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new MyItemDelegate(this));
    ui->label_record_num->setText("共" + QString::number(model->rowCount()) + "条记录");

    // 处理modelSto
    modelSto = new QSqlRelationalTableModel(this);
    modelSto->setTable("Storage_info");
    modelSto->setRelation(4, QSqlRelation("Sys_Seller", "Seller_Id", "Seller_name"));
    modelSto->setHeaderData(0, Qt::Horizontal, "仓库ID");
    modelSto->setHeaderData(1, Qt::Horizontal, "仓库名");
    modelSto->setHeaderData(2, Qt::Horizontal, "仓库容量");
    modelSto->setHeaderData(3, Qt::Horizontal, "剩余空间");
    modelSto->setHeaderData(4, Qt::Horizontal, "拥有者");
    modelSto->select();
    ui->tableView_sto->setModel(modelSto);

    // 信号与槽的连接
    connect(queryDialog, SIGNAL(sendMsg(QString, QString)),
            this, SLOT(getQuery(QString, QString)));
    connect(insertDialog, SIGNAL(sendMsg(QString, QString, QString)),
            this, SLOT(getInsert(QString, QString, QString)));

    // 是否为管理员
    if (0 == Data::is_admin) {
        ui->pushBtn_query->hide();
        ui->pushBtn_query_all->hide();
        ui->tabWidget->setTabEnabled(1, false);
        ui->tabWidget->setTabEnabled(2, false);
        query.exec(QString("select storageName from Storage_info "
                           "where sellerID=%1").arg(QString::number(User::id)));
    } else {
        query.exec("select storageName from Storage_info");
    }

    // 设置仓库combox数值
    while (query.next()) {
        ui->comboBox_sto->addItem(query.value(0).toString());
    }

}

StorageDialog::~StorageDialog()
{
    delete ui;
    delete model;
}

// 仓库选择
void StorageDialog::on_comboBox_sto_activated(const QString &stoName)
{
    if ("全部" == stoName) {
        model->setQuery("select * from Main_view", db);
        int rowCount = model->rowCount();
        ui->label_remain_num->setText("-/-");
        ui->label_record_num->setText("共" + QString::number(rowCount) + "条记录");
        return;
    }

    // 在仓库Combox中选中仓库后，显示相应信息
    QString stoID;
    QSqlQuery query(db);
    model->setQuery(QString("select * from Main_view "
                            "where storageName='%1'").arg(stoName), db);
    query.exec(QString("select storageID from Storage_info "
                       "where storageName='%1'").arg(stoName));
    if (query.next()) {
        stoID = query.value(0).toString();
    }
    query.exec(QString("select remain,capacity from Storage_info "
                       "where storageID=%1").arg(stoID));
    if (query.next()) {
        QString remain, record;
        remain += query.value(0).toString() + "/" + query.value(1).toString();
        record += "共" + query.value(0).toString() + "条记录";
        ui->label_remain_num->setText(remain);
        ui->label_record_num->setText(record);
    }

}

// 查询所有
void StorageDialog::on_pushBtn_query_all_clicked()
{
    model->setQuery("select * from Main_view", db);
    if (model->lastError().isValid()) qDebug() << model->lastError().text();
}

// 查询
void StorageDialog::on_pushBtn_query_clicked()
{
    queryDialog->show();
}

void StorageDialog::getQuery(QString option, QString content)
{
    if ("卖家姓名" == option) {
        model->setQuery(QString("select * from Main_view "
                                "where sellerName='%1'").arg(content), db);
    } else {
        model->setQuery(QString("select * from Main_view "
                                "where productName='%1'").arg(content), db);
    }
    QString record;
    record += "共" + QString::number(model->rowCount()) + "条记录";
    ui->label_record_num->setText(record);
    ui->label_remain_num->setText("-/-");
}

// 插入记录
void StorageDialog::on_pushBtn_insert_clicked()
{
    insertDialog->show();
}

void StorageDialog::getInsert(QString stoName, QString proName, QString amount)
{
    QString stoID;
    QString proID;
    QSqlQuery query(db);

    // 检查所填仓库是否存在
    query.exec(QString("select storageID from Storage_info "
                       "where storageName='%1'").arg(stoName));
    if (!query.next()) {
        ui->label_prompt_1->setText("提示：所填仓库不存在！");
        return;
    }
    stoID = query.value(0).toString();

    // 检查所填商品是否存在
    query.exec(QString("select id from stock_provider_product "
                       "where name='%1'").arg(proName));
    if (!query.next()) {
        ui->label_prompt_1->setText("提示：所填商品不存在！");
        return;
    }
    proID = query.value(0).toString();

    // 检查商品数量是否合法
    query.exec(QString("select remain from Storage_info "
                       "where storageID=%1").arg(stoID));
    query.next();
    int remainSpace = query.value(0).toInt();
    if (QString(amount).toInt() > remainSpace) {
        ui->label_prompt_1->setText("提示：仓库空间不足！");
        return;
    }

    // 修改仓库剩余空间
    query.exec(QString("update Storage_info "
                       "set remain=remain-%1 "
                       "where storageID=%2").arg(amount, stoID));

    // 检查仓库中是否已有该记录
    query.exec(QString("select * from Storage_product "
                       "where storageID=%1 and productID=%2").arg(stoID, proID));
    if (query.next()) {
        // 修改库存数据
        query.exec(QString("update Storage_product "
                           "set amount=amount+%1 "
                           "where storageID=%2 and productID=%3").arg(
                       amount, stoID, proID));
    } else {
        // 插入库存数据
        query.exec(QString("insert into Storage_product "
                           "values(%1,%2,%3)").arg(stoID, proID, amount));
    }

    if (query.lastError().isValid()) {
        qDebug() << query.lastError().text();
        ui->label_prompt_1->setText("提示：添加失败！");
    } else {
        ui->label_prompt_1->setText("提示：添加记录成功！");
    }
    model->setQuery("select * from Main_view", db);
}

// 删除一条记录
void StorageDialog::on_pushBtn_del_clicked()
{
    int ok = QMessageBox::warning(this, "删除当前行", "是否删除？",
                                  QMessageBox::Yes, QMessageBox::No);
    if (ok == QMessageBox::No) {
        ui->label_prompt_1->setText("提示：操作已取消！");
        return;
    }
    QString stoID;
    QString proID;
    QModelIndex curIndex = ui->tableView->currentIndex();
    QString stoName = model->data(model->index(curIndex.row(), 0)).toString();
    QString proName = model->data(model->index(curIndex.row(), 2)).toString();
    QString amount = model->data(model->index(curIndex.row(), 3)).toString();
    qDebug() << stoName << proName;
    QSqlQuery query(db);

    // 获取仓库ID
    query.exec(QString("select storageID from Storage_info "
                       "where storageName='%1'").arg(stoName));
    query.next();
    stoID = query.value(0).toString();

    // 获取商品ID
    query.exec(QString("select id from stock_provider_product "
                       "where name='%1'").arg(proName));
    query.next();
    proID = query.value(0).toString();

    // 修改仓库库存
    query.exec(QString("update Storage_info "
                       "set remain=remain+%1 "
                       "where storageID=%2").arg(amount, stoID));

    // 删除库存记录
    query.exec(QString("delete from Storage_product "
                       "where storageID=%1 and productID=%2").arg(stoID, proID));

    if (query.lastError().isValid()) {
        qDebug() << query.lastError().text();
        ui->label_prompt_1->setText("提示：删除失败！");
    } else {
        ui->label_prompt_1->setText("提示：删除记录成功！");
    }
    model->setQuery("select * from Main_view", db);
}

// 出库入库选择
void StorageDialog::on_comboBox_op_activated(const QString &choice)
{
    if ("出库" == choice) {
        ui->label_product->hide();
        ui->lineEdit_product->hide();
    } else if ("入库" == choice) {
        ui->label_product->show();
        ui->lineEdit_product->show();
    }
}

// 订单状态确认
void StorageDialog::on_pushBtn_confirm_clicked()
{
    // 出库操作
    if (ui->comboBox_op->currentText() == "出库") {
        QString orderID = ui->lineEdit_record->text();
        QString sellerID;
        QString productID;
        int amount;

        QSqlQuery query(db);
        query.exec(QString("select sellerID,productID,amount from Storage_wait_product "
                           "where orderID='%1'").arg(orderID));
        if (!query.next()) {
            ui->label_prompt_2->setText("提示：订单号不存在或已完成出库！");
            return;
        }
        sellerID = query.value(0).toString();
        productID = query.value(1).toString();
        amount = query.value(2).toInt();

        // 修改数据库
        // dec用于保存每个仓库出库后的剩余所需值
        int dec = amount;
        query.exec(QString("select sp.storageID from Storage_product sp, Storage_info si "
                           "where sp.storageID=si.storageID "
                           "and si.sellerID=%1 "
                           "and sp.productID=%2").arg(sellerID, productID));
        // 对于每一条记录
        while (dec > 0 && query.next()) {
            QString stoID = query.value(0).toString();
            int curAmount = 0;
            QSqlQuery modify(db);
            modify.exec(QString("select amount from Storage_product "
                                "where storageID=%1 and productID=%2").arg(stoID, productID));
            if (modify.next()) curAmount = modify.value(0).toInt();
            // 若该记录中商品数量不大于dec，则删除该条记录
            if (curAmount <= dec) {
                modify.exec(QString("delete from Storage_product "
                                    "where storageID=%1 and productID=%2").arg(stoID, productID));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            } else {
                // 否则修改该条记录的剩余数量
                modify.exec(QString("update Storage_product "
                                    "set amount=%1 "
                                    "where storageID=%2 and productID=%3").arg(
                                QString::number(curAmount - dec), stoID, productID));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            }
            // 修改仓库剩余空间值
            modify.exec(QString("update Storage_info "
                                "set remain=remain+%1 "
                                "where storageID=%2").arg(
                            QString::number(StorageManage::min(curAmount, dec)), stoID));
            if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            dec -= curAmount;
        }
        //qDebug() << dec;

        // 向出库记录表中增添一条记录
        QDateTime curTime = QDateTime::currentDateTime();
        query.exec(QString("insert into Storage_order_record "
                           "values('%1',%2,'%3',%4)").arg(
                       orderID,
                       productID,
                       curTime.toString(),
                       QString::number(amount)));
        if (query.lastError().isValid()) qDebug() << query.lastError().text();

        // 删除待出库表中的记录
        query.exec(QString("delete from Storage_wait_product "
                           "where orderID='%1'").arg(orderID));
        if (query.lastError().isValid()) qDebug() << query.lastError().text();

        // 调用接口修改订单状态
        Sale_Widget::Sale_State_Out(orderID);

        ui->label_prompt_2->setText("提示：出库成功!");
    } else {
        // 入库操作
        if (StorageManage::getOccu() <= 0) {
            ui->label_prompt_2->setText("提示：无未完成进货订单!");
            return;
        }
        QString orderID = ui->lineEdit_record->text();
        QString productID = ui->lineEdit_product->text();

        // 调用接口修改订单状态并获取订单内商品所属卖家和数量
        QStringList list = stock_MainWindow::stock_change_PlanState(
                               orderID.toInt(), productID.toInt());
        if (list.empty()) {
            qDebug() << "stock_change_PlanState返回的list为空";
            ui->label_prompt_2->setText("提示：输入数据有误！");
            return;
        }
        QString sellerID = list.at(0);
        int amount = list.at(1).toInt();

        QSqlQuery query(db);
        QSqlQuery modify(db);
        int dec = amount;
        query.exec(QString("select storageID from Storage_info "
                           "where sellerID=%1").arg(sellerID));
        while (dec > 0 && query.next()) {
            QString stoID = query.value(0).toString();
            modify.exec(QString("select remain from Storage_info "
                                "where storageID=%1").arg(stoID));
            modify.next();
            int remain = modify.value(0).toInt();
            // 该仓库剩余空间为0
            if (remain == 0) {
                continue;
            }
            // 该仓库空间小于或等于商品数量
            if (remain <= dec) {
                // 修改仓库空间信息
                modify.exec(QString("update Storage_info "
                                    "set remain=0 "
                                    "where storageID=%1").arg(stoID));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            } else {
                // 该仓库空间大于商品数量
                // 修改仓库空间信息
                modify.exec(QString("update Storage_info "
                                    "set remain=%1 "
                                    "where storageID=%2").arg(
                                QString::number(remain - dec), stoID));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            }
            // 检查该仓库中是否已有同种商品
            modify.exec(QString("select * from Storage_product "
                                "where storageID=%1 and productID=%2").arg(stoID, productID));
            if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            // 若该仓库中已有同种商品，则直接修改相应记录
            int temp = remain < dec ? remain : dec;
            if (modify.next()) {
                modify.exec(QString("update Storage_product "
                                    "set amount=amount+%1 "
                                    "where storageID=%2 and productID=%3").arg(
                                QString::number(temp), stoID, productID));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            } else {
                qDebug() << "test";
                // 若该仓库中无同种商品，则添加记录
                modify.exec(QString("insert into Storage_product "
                                    "values(%1,%2,%3)").arg(
                                stoID, productID, QString::number(temp)));
                if (modify.lastError().isValid()) qDebug() << modify.lastError().text();
            }
            dec -= remain;
        }
        // 修改临时占据空间变量
        //StorageManage::tempOccupied -= amount;
        StorageManage::changeOccu(StorageManage::getOccu() - amount);
        qDebug() << "tempOccupied:" << StorageManage::getOccu();
        ui->label_prompt_2->setText("提示：出库成功!");
    }
}

// 刷新仓库界面
void StorageDialog::on_pushBtn_refresh_clicked()
{
    modelSto->select();
}
