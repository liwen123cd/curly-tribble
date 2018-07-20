#include "sale_detail_dialog.h"
#include "sys_sqlite.h"
#include "ui_sale_detail_dialog.h"
#include "globaldata.h"
#include "storagemanage.h"
#include "stock_mainwindow.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QString>
#include <QPixmap>
#include <QPicture>
#include <QLabel>
//构造函数
Sale_Detail_Dialog::Sale_Detail_Dialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint),
    ui(new Ui::Sale_Detail_Dialog)
{
    ui->setupUi(this);

    if (!Sale_New_Table()) {
        qDebug() << tr("初始化失败");
    }
}
//析构函数
Sale_Detail_Dialog::~Sale_Detail_Dialog()
{
    delete ui;
}
//槽函数，确认按钮
void Sale_Detail_Dialog::on_Sale_enter_pushbutton_clicked()
{
    //判断非空
    if (Sale_Check_Detail()) {
        //给widget发数据
        if (Is_Dirty) {
            Sale_Order_Detail detail;
            detail.Sale_Order_ID = Order_ID;
            detail.Sale_Row = Row;
            detail.Sale_State = Order_State;
            detail.Sale_Buyer_Name = ui->Sale_buyer_name_lineedit->text();
            detail.Sale_Buyer_Tel = ui->Sale_buyer_tel_lineedit->text();
            detail.Sale_Buyer_Address = ui->Sale_buyer_address_lineedit->text();
            detail.Sale_Item_ID = ui->Sale_item_id_combobox->currentText().toInt();
            detail.Sale_Item_Num = ui->Sale_item_num_lineedit->text().toInt();
            detail.Sale_Item_Price = ui->Sale_item_price_lineedit->text().toFloat();
            detail.Sale_IS_Dirty = Is_Dirty;
            emit Sale_Send_Detail(detail);
        }
        Sale_Is_Check(true);
        hide();
    }


}
//槽函数取消按钮
void Sale_Detail_Dialog::on_Sale_cancel_pushbutton_clicked()
{
    //退出，给widget发信号，取消
    if (Order_ID == "") {
        emit Sale_Cancel(Row);
    }
    Sale_Is_Check(true);
    hide();
}
//槽函数，接收订单详细信息并显示
void Sale_Detail_Dialog::Sale_Recive_Detail(const Sale_Order_Detail &detail)
{
    //系统读入卖家信息（不可更改）
    if (!Data::is_admin) {
        ui->Sale_seller_name_lineedit->setText(User::name);
        ui->Sale_seller_tel_lineedit->setText(User::phone);
        ui->Sale_seller_address_lineedit->setText(User::addr);
    } else {
        //根据卖家id查卖家信息
        infoSeller(detail.Sale_Seller_ID);
        ui->Sale_seller_name_lineedit->setText(
                    infoSeller(detail.Sale_Seller_ID).name);
        ui->Sale_seller_tel_lineedit->setText(
                    infoSeller(detail.Sale_Seller_ID).phone);
        ui->Sale_seller_address_lineedit->setText(
                    infoSeller(detail.Sale_Seller_ID).addr);
    }


    //卖家输入买家信息，商品编号（判断是否有库存）（下拉框，可选），数量（大小范围限定0~库存数量），售价（暂定由卖家输入）
    //测试用
    QStringList items = StorageManage::getProductList();
    ui->Sale_item_id_combobox->clear();
    /*if (items.empty()) {
        ui->Sale_item_id_combobox->addItem(QString::number(detail.Sale_Item_ID));
    } else {*/
    if (!items.empty()) {
        ui->Sale_item_id_combobox->addItems(items);
    }

    //}

    //初始化,遍历一次库存列表，判断所选商品索引,库存,读入结构体中数据
    int count = StorageManage::getAmount(detail.Sale_Item_ID); //库存
    int num = 0; //索引，默认0
    if (detail.Sale_Item_ID != -1) {
        foreach (QString item_id, items) {
            if (item_id.toInt() == detail.Sale_Item_ID)break;
            ++num;
        }
        //如果库存中没有同类商品。添加
        if (num == items.count()) {
            ui->Sale_item_id_combobox->addItem(QString::number(
                                                   detail.Sale_Item_ID));
        }
    }
    //设置销售数量范围
    ui->Sale_item_num_lineedit->setValidator(
                new QIntValidator(0, count, this));
    //设置索引
    ui->Sale_item_id_combobox->setCurrentIndex(num);
    //添加信息
    ui->Sale_buyer_name_lineedit->setText(
                detail.Sale_Buyer_Name);
    ui->Sale_buyer_tel_lineedit->setText(
                detail.Sale_Buyer_Tel);
    ui->Sale_buyer_address_lineedit->setText(
                detail.Sale_Buyer_Address);
    ui->Sale_item_num_lineedit->setText(
                QString::number(detail.Sale_Item_Num));
    ui->Sale_item_price_lineedit->setText(
                QString::number(detail.Sale_Item_Price));
    Order_ID = detail.Sale_Order_ID;
    Row = detail.Sale_Row;
    Order_State = detail.Sale_State;
    Is_Dirty = detail.Sale_IS_Dirty;
    if (!Is_Dirty) {
        Sale_Is_Check(false);
    }
    //输入商品ID

    Sale_Show_Item(ui->Sale_item_id_combobox->currentText().toInt());
    //显示订单记录
    Sale_Show_State(detail.Sale_Order_ID);
}
//根据订单号显示订单状态表
bool Sale_Detail_Dialog::Sale_Show_State(const QString &Order_ID)
{
    //sql语句，根据订单号查订单记录


    Sale_Table_Model->setFilter(QString("Sale_Order_Id='%1'").arg(Order_ID));
    Sale_Table_Model->select();
    return true;
}
//显示商品详细信息
bool Sale_Detail_Dialog::Sale_Show_Item(int Item_ID)
{
    //显示商品详细信息
    //测试
    //系统显示商品详细信息（以后目标）调用接口读入
    if (Item_ID == -1) {
        ui->Sale_item_name_lineEdit->setText(tr("商品名"));
        ui->Sale_item_count_lineEdit->setText(tr("库存"));
        ui->Sale_item_purchase_price_lineEdit->setText(tr("进价"));
        ui->Sale_item_provider_lineEdit->setText(tr("供货商"));
        ui->Sale_item_pic->clear();
        return true;
    }
    Product_Detail detail;
    stock_MainWindow::stock_get_ProductDetail(detail, Item_ID);
    ui->Sale_item_name_lineEdit->setText(detail.Product_Name);
    //qDebug()<<StorageManage::getAmount(Item_ID);
    ui->Sale_item_count_lineEdit->setText(QString::number(StorageManage::getAmount(Item_ID)));
    ui->Sale_item_purchase_price_lineEdit->setText(QString::number(detail.Product_Price));
    ui->Sale_item_provider_lineEdit->setText(detail.Product_Provider);
    QPixmap pix;
    if (!pix.load(detail.Path)) {
        qDebug() << "打开文件失败";
    }
    ui->Sale_item_pic->clear();
    ui->Sale_item_pic->setPixmap(pix);
    return true;
}
//监测函数，判断输入内容是否合法
bool Sale_Detail_Dialog::Sale_Check_Detail()
{
    if (ui->Sale_buyer_name_lineedit->text() == "") {
        QMessageBox::warning(this, tr("警告"), tr("买家姓名不能为空"), QMessageBox::Ok);
        return false;
    }
    if (ui->Sale_buyer_tel_lineedit->text().size() != 11) {
        QMessageBox::warning(this, tr("警告"), tr("请填入有效手机号码"), QMessageBox::Ok);
        return false;
    }
    if (ui->Sale_buyer_address_lineedit->text() == "") {
        QMessageBox::warning(this, tr("警告"), tr("买家地址不能为空"), QMessageBox::Ok);
        return false;
    }
    if (ui->Sale_item_id_combobox->currentText() == "-1") {
        QMessageBox::warning(this, tr("警告"), tr("请选择有效的商品"), QMessageBox::Ok);
        return false;
    }
    if (ui->Sale_item_num_lineedit->text().toInt() < 1) {
        QMessageBox::warning(this, tr("警告"), tr("商品数量超出范围"), QMessageBox::Ok);
        return false;
    }
    if (ui->Sale_item_price_lineedit->text().toFloat() <= 0) {
        QMessageBox::warning(this, tr("警告"), tr("售价超出范围"), QMessageBox::Ok);
        return false;
    }
    return true;

}
//设置信息是否改
void Sale_Detail_Dialog::Sale_Is_Check(bool check)
{
    ui->Sale_buyer_name_lineedit->setEnabled(check);
    ui->Sale_buyer_tel_lineedit->setEnabled(check);
    ui->Sale_buyer_address_lineedit->setEnabled(check);
    ui->Sale_item_id_combobox->setEnabled(check);
    ui->Sale_item_num_lineedit->setEnabled(check);
    ui->Sale_item_price_lineedit->setEnabled(check);
}

//槽函数，读取当前选中商品信息
void Sale_Detail_Dialog::on_Sale_item_id_combobox_currentIndexChanged(const QString &Item_ID)
{
    int count = StorageManage::getAmount(Item_ID.toInt());
    //根据商品编号查商品信息(商品名，库存，供货商，进价）

    Sale_Show_Item(Item_ID.toInt());
    //重设销售数量范围
    ui->Sale_item_num_lineedit->setValidator(
                new QIntValidator(0, count, this));
}
//初始化sql模型
bool Sale_Detail_Dialog::Sale_New_Table()
{
    Sale_Table_Model = new QSqlTableModel(this);
    Sale_Table_Model->setTable("Sale_State");
    Sale_Table_Model->select();
    if (Sale_Table_Model->lastError().number() != -1) {
        return false;
    }
    Sale_Table_Model->setHeaderData(0, Qt::Horizontal, tr("订单状态ID"));
    Sale_Table_Model->setHeaderData(1, Qt::Horizontal, tr("订单号"));
    Sale_Table_Model->setHeaderData(2, Qt::Horizontal, tr("物流记录"));
    Sale_Table_Model->setHeaderData(3, Qt::Horizontal, tr("日期"));

    Sale_Table_Model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setModel(Sale_Table_Model);
    ui->Sale_item_pic->setScaledContents(true);
    ui->Sale_item_pic->resize(ui->widget->size());
    return true;
}


