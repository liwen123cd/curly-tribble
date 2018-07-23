/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：usermanager.h
  * @brief 内容: 管理用户头文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-8
  *
  * */
#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QWidget>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include "sys_add_user.h"

class QSqlTableModel;
namespace Ui
{
class UserManager;
}

class UserManager : public QWidget
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = 0);
    ~UserManager();

    void sqLite();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::UserManager *ui;
    QSqlTableModel *model;
    sys_add_user *addUser;
    void tableModel();
};

#endif // USERMANAGER_H
