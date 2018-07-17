#include "stock_mainwindow.h"
#include "ui_stock_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QSqlQuery>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QSqlError>
static float prev_price=0.0;
void stock_MainWindow::stock_provider_model_init()
{

    stock_provider->setTable("stock_provider");
    stock_provider->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView_3->setModel(stock_provider);
    stock_provider_select();
    ui->tableView_3->hideColumn(0);
    ui->tableView_3->hideColumn(1);
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stock_provider_product->setTable("stock_provider_product");
    stock_provider_product->setEditStrategy(QSqlTableModel::OnManualSubmit);
    stock_provider_product->select();
    ui->tableView_4->setModel(stock_provider_product);
    ui->tableView_4->hideColumn(0);
    ui->tableView_4->hideColumn(1);
    ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stock_tableview_3_clicked(stock_provider->index(0, 0));
    stock_set_headname();

}
void stock_MainWindow::stock_provider_select()
{
    if(!is_admin)
    {
        stock_provider->setFilter(tr("owner_id=%1 or owner_id=%2").arg(user_id).arg(GLOBAL_USER));
    }
    stock_provider->select();
}

void stock_MainWindow::stock_set_headname()
{
    stock_provider->setHeaderData(2, Qt::Horizontal, tr("供货商名称"));
    stock_provider->setHeaderData(3, Qt::Horizontal, tr("供货商地址"));
    stock_provider_product->setHeaderData(2, Qt::Horizontal, tr("商品名称"));
    stock_provider_product->setHeaderData(3, Qt::Horizontal, tr("商品价格"));
    stock_provider_product->setHeaderData(4,Qt::Horizontal,tr("商品图片"));
}
void stock_MainWindow::set_provider_visible(bool b)
{
    ui->label_9->setVisible(!b);
    ui->label_10->setVisible(!b);
    ui->label_11->setVisible(!b);
    ui->lineEdit_2->setVisible(!b);
    ui->lineEdit_3->setVisible(!b);
    ui->tableWidget->setVisible(!b);
    ui->pushButton_4->setVisible(!b);
    ui->label_7->setVisible(b);
    ui->label_8->setVisible(b);
    ui->tableView_3->setVisible(b);
    ui->tableView_4->setVisible(b);
    if(!is_admin)//不是管理员则可见
    {
        ui->pushButton_7->setVisible(b);
    }
    else//是管理员则把其设置为行选择
    {
        ui->tableView_3->setSelectionBehavior(QTableView::SelectRows);
        ui->tableView_4->setSelectionBehavior(QTableView::SelectRows);
    }
    set_provider_modify(!b);
    ui->widget->setVisible(b);
    ui->widget_2->setVisible(!b);
}
void stock_MainWindow::set_provider_modify(bool b)
{
    ui->pushButton_4->setVisible(b);
    ui->pushButton_5->setVisible(b);
    ui->pushButton_6->setVisible(b);
    if (ui->comboBox_2->currentIndex() == 0) {
        ui->pushButton_8->setVisible(b);

    } else {
        ui->pushButton_8->setVisible(false);
    }

}
void stock_MainWindow::on_pushButton_4_clicked()
{
    QString name;
    QString address;
    QSqlQuery query;
    int provider_id;
    int ok;
    int index = ui->comboBox_2->currentIndex();
    switch (index) {
    case 0:
        stock_provider_product->database().transaction();
        if (stock_provider_product->submitAll()) {
            stock_provider_product->database().commit();
        } else {
            qDebug()<<stock_provider_product->database().lastError();
            QMessageBox::warning(this, tr("错误"), tr("由于未知的原因（很可能是id重复），添加商品信息失败！"));
            stock_provider_product->database().rollback();
            qDebug() << "save fail";
        }
        break;
    case 1:
        name = ui->lineEdit_2->text();
        if (name == "") {
            QMessageBox::warning(this, tr("错误"), tr("供应商名称不能为空"));
            return;
        }
        address = ui->lineEdit_3->text();
        if (address == "") {
            QMessageBox::warning(this, tr("错误"), tr("供应商地址不能为空"));
            return;
        }
        query.exec(QString("select * from stock_provider where name = '%1'").arg(name));
        qDebug() << query.lastError();
        if (query.next()) {
            QMessageBox::warning(this, tr("错误"), tr("供应商已经存在，请到查询页面进行修改"));
            return;
        }
        provider_id = stock_make_id(3);
        stock_provider_product->database().transaction();
        ok = QMessageBox::warning(this, tr("提醒"), tr("你想把这个供货商信息共享给其他卖家吗？"), QMessageBox::Yes, QMessageBox::No);
        query.prepare("insert into stock_provider(id,owner_id,name,address) values(?,?,?,?)");
        query.addBindValue(provider_id);
        if (ok != QMessageBox::Yes) {
            query.addBindValue(user_id);
        } else {
            query.addBindValue(GLOBAL_USER);
        }
        query.addBindValue(name);
        query.addBindValue(address);
        if(!query.exec())
        {
            QMessageBox::warning(this, tr("错误"), tr("由于未知的原因（很可能是id重复），添加供货商失败！"));
            stock_provider_product->database().rollback();
            qDebug() << query.lastError();
            return;
        }
        qDebug() << query.lastError();
        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            int id = stock_make_id(5 + i);
            QString name;
            float price;
            if (ui->tableWidget->item(i, 0) != NULL) {
                name = ui->tableWidget->item(i, 0)->text();
            } else {
                QMessageBox::warning(this, tr("错误"), tr("商品名称不能为空"));
                stock_provider_product->database().rollback();
                return;
            }
            if (ui->tableWidget->item(i, 1) != NULL) {
                price = ui->tableWidget->item(i, 1)->text().toFloat();
            } else {
                QMessageBox::warning(this, tr("错误"), tr("商品价格不能为空"));
                stock_provider_product->database().rollback();
                return;
            }
            QString src_path=ui->tableWidget->item(i,2)->text();//图片源路径
            QString dest_path;//图片目的路径
             QString file_type;//图片文件的类型
            if(src_path=="") dest_path="../curly-tribble/img/product/na.jpg";
            else
            {
                if(src_path.endsWith(".png"))  file_type=".png";//因为我只允许添加这两种类型的文件，所以一个判断即可
                else  file_type=".jpg";
                dest_path=QString("../curly-tribble/img/product/product_")+QString::number(id)+file_type;
            }
            query.prepare("insert into stock_provider_product(id,provider_id,name,price,path) values(?,?,?,?,?)");
            query.addBindValue(id);
            query.addBindValue(provider_id);
            query.addBindValue(name);
            query.addBindValue(price);
            query.addBindValue(dest_path);
            while(!query.exec())
            {
                id=stock_make_id(5+i);
                query.bindValue(0,id);
                if(src_path!="") dest_path=QString("../curly-tribble/img/product/product_")+QString::number(id)+file_type;
                query.bindValue(4,dest_path);
                qDebug() << query.lastError();
            }
            if(!stock_save_ProductPicture(src_path,dest_path))
            {
                stock_provider_product->database().rollback();
                return;
            }
        }
        stock_provider_product->database().commit();
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        break;
    default:
        break;
    }
}
/*这个函数负责把用户提供的图片文件存放在项目的img文件夹下
 *
 */
bool stock_MainWindow::stock_save_ProductPicture(QString src_path,QString dest_path)
{
    if(src_path=="") return true;
    QFile src_file(src_path);
    QFile dest_file(dest_path);
    char buffer[4096];
    if(!src_file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("错误"), tr("打开源图片文件出错！！！"));
        qDebug()<<src_path;
        return false;
    }
    if(!dest_file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("错误"), tr("创建目的图片文件出错！！！"));
        qDebug()<<dest_path;
        return false;
    }
    while(!src_file.atEnd())
    {
        int ret=src_file.read(buffer,4096);
        dest_file.write(buffer,ret);
    }
    return true;

}

void stock_MainWindow::on_pushButton_5_clicked()
{

    int index = ui->comboBox_2->currentIndex();
    int rowNum;
    int id;
    int provider_id;
    int provider_row = ui->tableView_3->currentIndex().row();
    switch (index) {
    case 0:
        rowNum = stock_provider_product->rowCount();
        id = stock_make_id(5);
        stock_provider_product->insertRow(rowNum);
        provider_id = stock_provider->data(stock_provider->index(provider_row, 0)).toInt();
        stock_provider_product->setData(stock_provider_product->index(rowNum, 0), id);
        stock_provider_product->setData(stock_provider_product->index(rowNum, 1), provider_id);
        break;
    case 1:
        rowNum=ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowNum+ 1);
        ui->tableWidget->setItem(rowNum,2,new QTableWidgetItem());
        ui->tableWidget->item(rowNum,2)->setFlags(Qt::NoItemFlags);
        break;
    default:
        break;
    }
}
void stock_MainWindow::on_pushButton_6_clicked()
{
    int index = ui->comboBox_2->currentIndex();
    int row;
    int ok;
    switch (index) {
    case 0:
        row = ui->tableView_4->currentIndex().row();
        ok = QMessageBox::warning(this, tr("删除当前商品！"), tr("你确定删除当前商品吗？"), QMessageBox::Yes, QMessageBox::No);
        if (ok == QMessageBox::Yes) {
            stock_provider_product->removeRow(row);
            stock_provider_product->submitAll();
        }
        break;
    case 1:
        row = ui->tableWidget->currentRow();
        if (row != -1) {
            ui->tableWidget->removeRow(row);
        }
        break;
    default:
        break;
    }
}
void stock_MainWindow::on_pushButton_8_clicked()
{
    int row = ui->tableView_3->currentIndex().row();
    int provider_id = stock_provider->data(stock_provider->index(row, 0)).toInt();
    QMessageBox box(QMessageBox::Warning, tr("删除当前供货商！"), tr("你确定删除当前供货商吗？"),QMessageBox::NoButton,this);
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    box.setButtonText(QMessageBox::Yes, tr("确认"));
    box.setButtonText(QMessageBox::No, tr("取消"));
    int ok = box.exec();
    if (ok == QMessageBox::Yes) {
        QSqlQuery query;
        query.exec(QString("delete from stock_provider_product where provider_id=%1").arg(provider_id));
        qDebug() << query.lastError();
        stock_provider->removeRow(row);
        stock_provider->submitAll();
        stock_refresh_provider();
    }
}
void stock_MainWindow::on_pushButton_7_clicked()
{
    if (!ui->pushButton_4->isVisible()) {
        set_provider_modify(true);
        ui->tableView_3->setEditTriggers(QAbstractItemView::AllEditTriggers);
        ui->tableView_4->setEditTriggers(QAbstractItemView::AllEditTriggers);
    } else {
        set_provider_modify(false);
        ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}
void stock_MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        set_provider_visible(true);
        stock_refresh_provider();
        break;
    case 1:
        set_provider_visible(false);
        break;
    default:
        break;
    }
}
void stock_MainWindow::stock_refresh_provider()
{
    stock_provider_select();
    ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stock_tableview_3_clicked(stock_provider->index(0, 0));
    stock_set_headname();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();
}
void stock_MainWindow::stock_tableview_3_clicked(const QModelIndex&index)
{

    int provider_id = stock_provider->data(stock_provider->index(index.row(), 0)).toInt();
    stock_provider_product->setFilter(tr("provider_id='%1'").arg(provider_id));
    stock_provider_product->select();

}
void stock_MainWindow::on_pushButton_15_clicked()
{
    on_pushButton_6_clicked();
}
void stock_MainWindow::on_pushButton_16_clicked()
{
    on_pushButton_4_clicked();
}
void stock_MainWindow::on_pushButton_17_clicked()
{
    on_pushButton_5_clicked();
}

void stock_MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if(column!=2) return;
    else
    {
        QString filename=QFileDialog::getOpenFileName(this,tr("请选择图片文件"),"F:",tr("图片文件(*png *jpg)"));
        qDebug()<<filename;
        ui->tableWidget->item(row,column)->setText(filename);
    }
}


void stock_MainWindow::on_tableView_4_pressed(const QModelIndex &index)
{
    if(!ui->pushButton_4->isVisible()) return;
    if(index.column()==3)
    {
        prev_price=stock_provider_product->data(index).toFloat();//点击时，则记录以前的价格，以便以后恢复
        return;
    }
    if(index.column()!=4) return;
    QString filename=QFileDialog::getOpenFileName(this,tr("请选择图片文件"),"F:",tr("图片文件(*png *jpg)"));
    qDebug()<<filename;
    if(filename=="") return;
    QString dest_path=stock_provider_product->data(index).toString();
    if(dest_path=="../curly-tribble/img/product/na.jpg"||dest_path=="")
    {
        int id=stock_provider_product->data(stock_provider_product->index(index.row(),0)).toInt();
        QString file_type;//图片文件的类型
        if(filename.endsWith(".png"))  file_type=".png";//因为我只允许添加这两种类型的文件，所以一个判断即可
        else  file_type=".jpg";
        dest_path=QString("../curly-tribble/img/product/product_")+QString::number(id)+file_type;
        stock_provider_product->setData(index,dest_path);
    }
    stock_save_ProductPicture(filename,stock_provider_product->data(index).toString());
}
//这个函数在添加供货商界面检测商品价格是否修改为负数
void stock_MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if(item->column()!=1) return;
    float price=item->text().toFloat();
    if(price<0)
    {
        QMessageBox::warning(this, tr("错误"), tr("商品价格不能为负数！！！"));
        disconnect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
        item->setText(QString::number(prev_price));
        connect(ui->tableWidget,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(on_tableWidget_itemChanged(QTableWidgetItem*)));
        return;
    }
}
void stock_MainWindow::stock_productPrice_changed(const QModelIndex &left,const QModelIndex &right)
{
    int left_index=left.column();
    int right_index=right.column();
    if(left_index!=3||right_index!=3) return;
    float price=stock_provider_product->data(left).toFloat();
    if(price<0)
    {
        QMessageBox::warning(this, tr("错误"), tr("商品价格不能为负数！！！"));
        disconnect(stock_provider_product,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(stock_productPrice_changed(QModelIndex,QModelIndex)));
        stock_provider_product->setData(left,prev_price);
        connect(stock_provider_product,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(stock_productPrice_changed(QModelIndex,QModelIndex)));
        return;
    }
}
//点击时，则记录以前的价格，以便以后恢复
void stock_MainWindow::on_tableWidget_pressed(const QModelIndex &index)
{
    if(index.column()==1)
    {
        if(ui->tableWidget->item(index.row(),index.column())!=NULL)//不为空则记录以前的值
        {
            prev_price=ui->tableWidget->item(index.row(),index.column())->text().toFloat();
        }
        else//为空就表明是新创建的，以前的值就设置为0
        {
            prev_price=0;
        }
    }
    return;
}

