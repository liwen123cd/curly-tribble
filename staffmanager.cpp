/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：StaffManager.cpp
  * @brief 内容: 管理职工源文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-17
  *
  * */
#include "staffmanager.h"
#include "ui_staffmanager.h"
#include "sys_add_staff.h"
#include "sys_maintain_staff.h"
#include "sys_sqlite.h"
#include "storagemanage.h"

StaffManager::StaffManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StaffManager)
{
    ui->setupUi(this);

    // 添加背景图片
    QPixmap _image;
    _image.load(":/img/login/log2.jpg");
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                                                        Qt::SmoothTransformation)));
    setPalette(pal);

    model = new QSqlTableModel(this);
    init();
    init2();
}

StaffManager::~StaffManager()
{
    delete ui;
}

// 添加新职工窗口
void StaffManager::on_pushButton_clicked()
{
    addStaff = new Sys_Add_Staff(this);
    addStaff->exec();
    model->select();
}

// 初始化界面操作
void StaffManager::on_pushButton_9_clicked()
{
    qDebug() << "选择全部";
    init();
}

/**
  * @functionName Function Name : on_pushButton_2_clicked()
  * @brief Description: 选中一行维护职工信息，牵扯到离职等
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void StaffManager::on_pushButton_2_clicked()
{
    int curRow = ui->tableView->currentIndex().row();
    qDebug() << curRow;
    if (-1 == curRow) {
        QMessageBox::warning(this, tr("提示"),
                             "请先选中一行！");
    } else {

        // 新生成一个职工实例保存选中行职工信息
        staff = new Staff;
        QSqlRecord record = model->record(curRow);
        staff->id = record.value(0).toString();
        staff->name = record.value(1).toString();
        staff->deperment = record.value(2).toString();
        staff->position = record.value(3).toString();
        staff->status = record.value(4).toString();
        staff->phone = record.value(5).toString();
        staff->date = record.value(6).toString();
        staff->image = record.value(7).toString();
        staff->description = record.value(8).toString();

        // 生成新的维护职工信息窗口
        mainTain = new Sys_Maintain_Staff(this);
        mainTain->exec();
        //  sms->exec();
    }
    model->select();
}

// 保存数据库修改操作
void StaffManager::on_pushButton_3_clicked()
{
    if (NULL == ui->tableView->currentIndex().data(0).toString()) {
        QMessageBox::warning(this, tr("提示"),
                             "请输入需要修改的内容！");
        model->revertAll();
    }
    // 开始事务操作
    model->database().transaction();

    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("警告"),
                             tr("数据库错误: %1")
                             .arg(model->lastError().text()));
    }
}

// 撤销数据库操作
void StaffManager::on_pushButton_4_clicked()
{
    model->revertAll();
}

// 查询职工信息
void StaffManager::on_pushButton_11_clicked()
{
    QString inputQuery = ui->lineEdit->text();
    if (NULL == inputQuery) {
        QMessageBox::warning(this, tr("提示"),
                             "请输入需要查询的内容！");
    } else {
        queryStaff(ui->lineEdit->text());
        ui->lineEdit->clear();
    }
}

/**
  * @functionName Function Name : queryStaff(QString input)
  * @brief Description: 根据输入查询职工的信息
  * @date Date: 2018-7-10
  * @param Parameter: input
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void StaffManager::queryStaff(QString input)
{
    QString comboxText = ui->comboBox->currentText();
    qDebug() << comboxText;
    if (STAFF_ID == comboxText) {
        if (checkExist(input, STAFF_ID_NUMBER)) {
            model->setFilter(QString("Staff_Id = '%1'")
                             .arg(input));
        } else {
            QMessageBox::warning(this, tr("提示"),
                                 "该职工不存在，请重试！");
        }
    }
    if (STAFF_NAME == comboxText) {
        if (checkExist(input, STAFF_NAME_NUMBER)) {
            model->setFilter(QString("Staff_Name = '%1'")
                             .arg(input));
        } else {
            QMessageBox::warning(this, tr("提示"),
                                 "该职工不存在，请重试！");
        }
    }
    if (STAFF_DEPENTMENT == comboxText) {
        if (checkExist(input, STAFF_DEPENTMENT_NUMBER)) {
            model->setFilter(QString("Staff_Deperment = '%1'")
                             .arg(input));
        } else {
            QMessageBox::warning(this, tr("提示"),
                                 "该部门无职工，请重试！");
        }
    }
}

/**
  * @functionName Function Name : init()
  * @brief Description: 初始化 QTableview 函数
  * @date Date: 2018-7-10
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void StaffManager::init()
{
    model = new QSqlTableModel(this);
    model->setTable("Sys_Staff");
    model->select();

    // 系统管理只能物流公司使用，即管理系统管理员
    model->setHeaderData(0, Qt::Horizontal, tr("编号"));
    model->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    model->setHeaderData(2, Qt::Horizontal, tr("所在部门"));
    model->setHeaderData(3, Qt::Horizontal, tr("职位"));
    model->setHeaderData(4, Qt::Horizontal, tr("状态"));
    model->setHeaderData(5, Qt::Horizontal, tr("电话"));
    model->setHeaderData(6, Qt::Horizontal, tr("就职日期"));
    model->setHeaderData(7, Qt::Horizontal, tr("照片"));
    model->setHeaderData(8, Qt::Horizontal, tr("备注"));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    // ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    // 职工ID隐藏，其余信息可修改
    ui->tableView->hideColumn(0);
    ui->tableView->setFocusPolicy(Qt::NoFocus);
}

// 删除离职职工信息
void StaffManager::on_pushButton_12_clicked()
{
    int curRow = ui->tableView->currentIndex().row();

    QString currentText = model->data(model->index(curRow, 4)).toString();
    qDebug() << currentText;
    if (-1 == curRow) {
        QMessageBox::warning(this, tr("提示"),
                             "请先选中一行！");
    } else {
        if (STATUS_LEAVE == currentText) {
            model->removeRow(curRow);
            int ok = QMessageBox::warning(this, tr("删除离职职工信息"),
                                          tr("你确定删除当前已离职的职工信息吗？"),
                                          QMessageBox::Yes,
                                          QMessageBox::No);
            if (ok == QMessageBox::No) {
                model->revertAll();
            } else {
                model->submitAll();
            }
        } else {
            QMessageBox::warning(this, tr("警告"),
                                 tr("该用户未离职，无法删除！")
                                 );
        }
    }
}

// 增加新职工窗口
void StaffManager::on_pushButton_5_clicked()
{
    addSeller = new Sys_Add_Seller(this);
    addSeller->exec();
    model2->select();
}

// 查询卖家信息窗口
void StaffManager::on_pushButton_13_clicked()
{
    QString inputQuery = ui->lineEdit_2->text();
    if (NULL == inputQuery) {
        QMessageBox::warning(this, tr("提示"),
                             "请输入需要查询的内容！");
    } else {
        querySeller(ui->lineEdit_2->text());
        ui->lineEdit_2->clear();
    }
}

/**
  * @functionName Function Name : querySeller(QString inputText)
  * @brief Description: 根据条件查询卖家信息
  * @date Date: 2018-7-4
  * @param Parameter: inptText
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void StaffManager::querySeller(QString inputText)
{
    QString comboxText = ui->comboBox_2->currentText();
    if (SELLER_ID == comboxText) {
        if (checkExist(inputText, SELLER_ID_NUMBER)) {
            qDebug() << inputText.toInt();
            model2->setFilter(QString("Seller_Id = '%1'")
                              .arg(inputText.toInt()));
        } else {
            QMessageBox::warning(this, tr("提示"),
                                 "该卖家不存在，请重试！");
        }
    }
    if (STORAGR_ID == comboxText) {
        if (checkExist(inputText, STORAGE_ID_NUMBER)) {
            model2->setFilter(QString("storageID = '%1'")
                              .arg(inputText.toInt()));
        } else {
            QMessageBox::warning(this, tr("提示"),
                                 "该仓库不存在，请重试！");
        }
    }
}

// 初始化卖家管理的 QTabeView
void StaffManager::on_pushButton_10_clicked()
{
    init2();
    model2->select();
}

/**
  * @functionName Function Name : init2()
  * @brief Description: 初始化卖家 QTableView 操作函数
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void StaffManager::init2()
{
    model2 = new QSqlTableModel(this);
    model2->setTable("Seller_View");
    model2->setFilter("Seller_Id != -1");
    model2->select();
    model2->setHeaderData(0, Qt::Horizontal, tr("卖家ID"));
    model2->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    model2->setHeaderData(2, Qt::Horizontal, tr("电话"));
    model2->setHeaderData(3, Qt::Horizontal, tr("地址"));
    model2->setHeaderData(4, Qt::Horizontal, tr("仓库ID"));
    model2->setHeaderData(5, Qt::Horizontal, tr("仓库名"));
    model2->setHeaderData(6, Qt::Horizontal, tr("余量"));
    model2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView_2->setModel(model2);
    ui->tableView_2->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_2->hideColumn(4);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setFocusPolicy(Qt::NoFocus);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
}

// 删除卖家的信息
void StaffManager::on_pushButton_6_clicked()
{
    int curRow = ui->tableView_2->currentIndex().row();

    int currentID = model2->data(model2->index(curRow, 0)).toInt();

    qDebug() << currentID;
    if (-1 == curRow) {
        QMessageBox::warning(this, tr("提示"),
                             "请先选中一行！");
    } else {
        model2->removeRow(curRow);
        int ok = QMessageBox::warning(this, tr("删除卖家信息"),
                                      tr("你确定删除当前卖家信息吗？"),
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if (ok == QMessageBox::No) {
            model2->revertAll();
        } else {
            //  调用 仓库的接口函数 回收卖家所拥有的仓库
            StorageManage::freeStorage(currentID);

            // 从数据库中删除卖家的信息
            QSqlQuery query;
            QString deleteSql1 = QString("delete "
                                         "from Sys_Seller "
                                         "where Seller_Id = :Seller_Id");
            query.prepare(deleteSql1);
            query.bindValue(":Seller_Id", currentID);
            if (!query.exec()) {
                QMessageBox::information(this, "提示",
                                         query.lastError().text());
                qDebug() << query.lastError();
            } else {
                QMessageBox::information(this, "提示",
                                         "您已成功删除卖家信息！");
            }
        }
    }
    model2->select();
}

/**
  * @functionName Function Name : on_pushButton_7_clicked()
  * @brief Description: 维护卖家信息
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void StaffManager::on_pushButton_7_clicked()
{
    int curRow = ui->tableView_2->currentIndex().row();
    qDebug() << curRow;
    if (-1 == curRow) {
        QMessageBox::warning(this, tr("提示"),
                             "请先选中一行！");
    } else {
        // 实例化一个新的卖家 保存现在信息
        seller = new Seller;
        QSqlRecord record = model2->record(curRow);
        seller->id = record.value(0).toInt();
        seller->name = record.value(1).toString();
        seller->phone = record.value(2).toString();
        seller->addr = record.value(3).toString();
        seller->number = record.value(4).toInt();

        // 打开新的维护卖家信息窗口
        mainTainSeller = new Sys_Maintain_Seller(this);
        mainTainSeller->exec();
    }
    model2->select();
}
