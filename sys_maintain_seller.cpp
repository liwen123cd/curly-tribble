#include "sys_maintain_seller.h"
#include "ui_sys_maintain_seller.h"
#include <QSqlQuery>
#include <QString>
#include <QSqlError>
#include <QDebug>

Sys_Maintain_Seller::Sys_Maintain_Seller(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sys_Maintain_Seller)
{
    ui->setupUi(this);

    sellerID = 0;   // 从上一个界面获取到的卖家ID
    QString stoPro;
    QString stofree;
    QSqlQuery query;

    // 查询拥有仓库
    query.exec("select storageName from Storage_info "
               "where sellerID=-1");
    while (query.next()) {
        stoPro += query.value(0).toString() + "\n";
    }
    query.exec(QString("select storageName from Storage_info "
                       "where sellerID=%1").arg(QString::number(sellerID)));

    // 查询空闲仓库
    while (query.next()) {
        stofree += query.value(0).toString() + "\n";
    }

    // 显示
    ui->textBrowser_pro->setText(stoPro);
    ui->textBrowser_free->setText(stofree);
}

Sys_Maintain_Seller::~Sys_Maintain_Seller()
{
    delete ui;
}

// 删除仓库槽函数
void Sys_Maintain_Seller::on_pushButton_del_clicked()
{
    QString target = ui->lineEdit_pro->text();
    QSqlQuery query;
    query.exec(QString("update Storage_info "
                       "set sellerID=-1 "
                       "where storageName='%1'").arg(target));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError().text();
    }
}

// 添加仓库槽函数
void Sys_Maintain_Seller::on_pushButton_add_clicked()
{
    QString target = ui->lineEdit_free->text();
    QSqlQuery query;
    query.exec(QString("update Storage_info "
                       "set sellerID=%1 "
                       "where storageName='%1'").arg(target));
    if (query.lastError().isValid()) {
        qDebug() << query.lastError().text();
    }
}
