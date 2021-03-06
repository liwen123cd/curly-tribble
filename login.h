/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：login.h
  * @brief 内容: 登录头文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-19
  *
  * */
#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QString>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QIcon>
#include "register.h"
#include "globaldata.h"
#include "mainwindow.h"
#include "recovepwd.h"

namespace Ui
{
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    //    bool userCheck();
public slots:
    void reshow();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Login *ui;
    MainWindow *mainw;
    Register *reg;
    RecovePwd *rp;
};

#endif // LOGIN_H
