#include "addstodialog.h"
#include "ui_addstodialog.h"
#include <QString>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AddStoDialog::AddStoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddStoDialog)
{
    ui->setupUi(this);
}

AddStoDialog::~AddStoDialog()
{
    delete ui;
}

// 确定
void AddStoDialog::on_buttonBox_accepted()
{
    QString stoName = ui->lineEdit_stoname->text();
    QString capacity = ui->spinBox_cap->text();

    // 仓库名为空
    if ("" == stoName) {
        QMessageBox::critical(this, "错误", "仓库名不能为空！");
        return;
    }

    // 仓库名是否已存在
    QSqlQuery query;
    query.exec(QString("select * from Storage_info "
                       "where storageName='%1'").arg(stoName));
    if (query.next()) {
        QMessageBox::critical(this, "错误", "该仓库名已存在！");
        return;
    }

    // 操作数据库
    query.exec(QString("insert into Storage_info(storageName,capacity,remain,sellerID) "
                       "values('%1',%2,%3,-1)").arg(
                   stoName, capacity, capacity));
    if (query.lastError().isValid()) {
        qDebug() << "添加仓库失败" << query.lastError().text();
        QMessageBox::critical(this, "错误", "添加仓库失败！");
    } else {
        qDebug() << "添加仓库成功";
        QMessageBox::information(this, "成功", "添加仓库成功！");
    }
    close();
}

// 取消
void AddStoDialog::on_buttonBox_rejected()
{
    close();
}
