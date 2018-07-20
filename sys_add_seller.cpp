/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：Sys_Add_Seller.cpp
  * @brief 内容: 添加卖家源文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-17
  *
  * */
#include "sys_add_seller.h"
#include "ui_sys_add_seller.h"
#include "storagemanage.h"

Sys_Add_Seller::Sys_Add_Seller(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sys_Add_Seller)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);
    model->setHeaderData(0, Qt::Horizontal, "仓库名");
    ui->listView->setModel(model);
}

Sys_Add_Seller::~Sys_Add_Seller()
{
    delete ui;
}

/**
  * @functionName Function Name : on_pushButton_clicked()
  * @brief Description: 添加卖家操作 要查看仓库是否可以分配
  *                     没有仓库无法添加卖家
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Add_Seller::on_pushButton_clicked()
{
    QString add_seller_name = ui->lineEdit->text();
    QString add_seller_phone = ui->lineEdit_2->text();
    QString add_seller_addr = ui->lineEdit_3->text();
    int add_seller_number = ui->spinBox->text().toInt();
    int sellerID = 0;

    if(NULL == add_seller_name)
        QMessageBox::warning(this, tr("提示"),
                             "请填写卖家姓名！");
    if(NULL == add_seller_phone)
        QMessageBox::warning(this, tr("提示"),
                             "请填写卖家电话！");
    if(NULL == add_seller_addr)
        QMessageBox::warning(this, tr("提示"),
                             "请填写卖家地址！");

    qDebug()<<add_seller_number;
    qDebug()<<StorageManage::restStorage();

    // 判断仓库数量是否满足 不满足添加不了用户
    // 不用检测卖家姓名，因为卖家区分是由ID辨别的
    if(NULL != add_seller_name && NULL != add_seller_phone
            && NULL != add_seller_addr &&
            add_seller_number <= StorageManage::restStorage()) {

        // 添加卖家信息到数据库
        QString add_sql = QString("insert into Sys_Seller("
                                  "Seller_Name, Seller_Phone, "
                                  "Seller_Addr)");
        add_sql += QString(" VALUES('%1','%2','%3')")
                .arg(add_seller_name)
                .arg(add_seller_phone)
                .arg(add_seller_addr);

        QSqlQuery query;

        if(!query.exec(add_sql))
        {
            QMessageBox::information(this, "警告",
                                     query.lastError().text());

        } else {
            // 获取当前卖家的自增主键ID
            query.exec("select last_insert_rowid() from Sys_Seller");
            while(query.next())
            {
                sellerID = query.value(0).toInt();
            }

        }

        // 调用仓库的分配函数，为卖家分配仓库
        int flag = StorageManage::allocateStorage(sellerID, add_seller_number);
        if(0 == flag && add_seller_number < StorageManage::restStorage()) {
            QMessageBox::information(this, "提示",
                                     "您已成功分配仓库！");
        } else if(0 == flag && add_seller_number == StorageManage::restStorage()){
            QMessageBox::information(this, "提示",
                                     "您未分配仓库！");
        }

        qDebug()<<sellerID;
        // 在界面显示仓库信息
        model->setQuery(QString("select storageName from Storage_info "
                                "where sellerID=%1").arg(QString::number(sellerID)));

        init();
        QMessageBox::information(this, "添加成功",
                                 "您已成功添加新用户！");
        this->close();
    }
    else {
        QMessageBox::information(this, "警告",
                                 "仓库不足，无法添加新用户！");
    }
}

// 初始化操作
void Sys_Add_Seller::on_pushButton_2_clicked()
{
    init();
}

// 重置选项
void Sys_Add_Seller::init()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->spinBox->setValue(0);
    ui->listView->clearMask();
}
