/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：register.cpp
  * @brief 内容: 注册功能
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-17
  *
  * */
#include "register.h"
#include "ui_register.h"
#include "sys_sqlite.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    // 设置背景图片
    this->setAutoFillBackground(true);
    QPixmap _image;
    _image.load(":/img/login/log2.jpg");
    QPalette pal(palette());
    pal.setBrush(QPalette::Window,
                 QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                                      Qt::SmoothTransformation)));
    setPalette(pal);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->label_6->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->label_7->setVisible(false);
    ui->lineEdit_6->setVisible(false);

}

Register::~Register()
{
    delete ui;
}

// 重写closeEvent 事件
void Register::closeEvent(QCloseEvent *event)
{
    event->accept();
    this->close();
    this->parentWidget()->show();
}

/**
  * @functionName Function Name : on_pushButton_clicked()
  * @brief Description: 注册操作主函数
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Register::on_pushButton_clicked()
{
    if (textCheck()) {
        QString reg_name = ui->lineEdit->text();
        QString reg_pwd = ui->lineEdit_2->text();
        QString reg_emial = ui->lineEdit_4->text();
        QString reg_phone = ui->lineEdit_5->text();
        QString reg_addr = ui->lineEdit_6->text();
        int admin;
        if (ui->comboBox->currentText() == "管理员")
            admin = 1;
        else
            admin = 0;

        if (userCheck(reg_name, SYS_USER_NUMBER)) {

            QString add_sql = QString("insert into Sys_User(User_Name, "
                                      "User_Pwd, User_Email, User_Is_Admin)");
            add_sql += QString(" VALUES('%1','%2','%3','%4')")
                    .arg(reg_name)
                    .arg(reg_pwd)
                    .arg(reg_emial)
                    .arg(admin);

            QSqlQuery query;
            query.exec(add_sql);

            // 如果是卖家，把信息添加到卖家信息表中
            if (admin == 0) {
                QString add_seller_sql = QString("insert into Sys_Seller( "
                                                 "Seller_Name, Seller_Phone, Seller_Addr) ");
                add_seller_sql += QString("VALUES('%1', '%2', '%3')")
                        .arg(reg_name)
                        .arg(reg_phone)
                        .arg(reg_addr);
                QSqlQuery query2;
                query2.exec(add_seller_sql);
            }

            QMessageBox::information(this, "注册成功",
                                     "您已成功注册新用户，请返回去登陆！");
            this->close();
        } else {
            QMessageBox::warning(this, "注册失败",
                                 "该用户已经存在，请更换用户名重试！");
        }
    }
}

// 重置操作
void Register::on_pushButton_2_clicked()
{
    ui->comboBox->setCurrentIndex(0);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
}

/**
  * @functionName Function Name : textCheck()
  * @brief Description: 增强程序鲁棒性，检测是否为空
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: true
  * @author Author: 张岩森
  *
  * */
bool Register::textCheck()
{
    if (NULL == ui->lineEdit->text())
        QMessageBox::warning(this, "警告",
                             "用户名为空，请输入用户名！");
    if (NULL == ui->lineEdit_2->text())
        QMessageBox::warning(this, "警告",
                             "密码为空，请输入密码！");
    if (NULL == ui->lineEdit_4->text())
        QMessageBox::warning(this, "警告",
                             "邮箱为空，请输入邮箱地址！");
    if (ui->lineEdit_2->text() != ui->lineEdit_3->text()) {
        ui->lineEdit_3->clear();
        QMessageBox::warning(this, "警告",
                             "两次密码输入不一致，请重新输入！");
    }
    if (ui->lineEdit->text() != NULL
            && ui->lineEdit_2->text() != NULL
            && ui->lineEdit_3->text() != NULL
            && ui->lineEdit_4->text() != NULL
            && ui->lineEdit_2->text() == ui->lineEdit_3->text())
        return true;
    return false;
}

/**
  * @functionName Function Name : on_comboBox_currentIndexChanged
  * @brief Description: 当组合框选择用户或卖家时，出现不同选项
  * @date Date: 2018-7-4
  * @param Parameter: index
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Register::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->label_6->setVisible(false);
        ui->lineEdit_5->setVisible(false);
        ui->label_7->setVisible(false);
        ui->lineEdit_6->setVisible(false);
    } else if (index == 1) {
        ui->label_6->setVisible(true);
        ui->lineEdit_5->setVisible(true);
        ui->label_7->setVisible(true);
        ui->lineEdit_6->setVisible(true);
    }
}
