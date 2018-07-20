/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：sys_add_seller.h
  * @brief 内容: 添加卖家头文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-19
  *
  * */
#ifndef SYS_ADD_SELLER_H
#define SYS_ADD_SELLER_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>

namespace Ui {
class Sys_Add_Seller;
}

class Sys_Add_Seller : public QDialog
{
    Q_OBJECT

public:
    explicit Sys_Add_Seller(QWidget *parent = 0);
    ~Sys_Add_Seller();

    void init();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Sys_Add_Seller *ui;
    QSqlQueryModel *model;
};

#endif // SYS_ADD_SELLER_H
