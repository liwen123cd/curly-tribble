/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：UserManager.cpp
  * @brief 内容: 用户管理源文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-17
  *
  * */
#include "usermanager.h"
#include "ui_usermanager.h"
#include "sys_sqlite.h"

UserManager::UserManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserManager)
{
    ui->setupUi(this);

    // 添加背景图片
    QPixmap _image;
    _image.load(":/img/login/log2.jpg");
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                                                        Qt::SmoothTransformation)));
    setPalette(pal);
    //    tableModel();

    tableModel();
}

UserManager::~UserManager()
{
    delete ui;
}

// 删除用户信息
void UserManager::on_pushButton_2_clicked()
{
    int curRow = ui->tableView->currentIndex().row();

    if (-1 == curRow) {
        QMessageBox::warning(this, tr("提示"),
                             "请先选中一行！");
    } else {
        model->removeRow(curRow);
        int ok = QMessageBox::warning(this, tr("删除当前用户？"),
                                      tr("你确定删除当前用户吗？"),
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if (ok == QMessageBox::No) {
            model->revertAll();
        } else {
            model->submitAll();
        }
    }
}

// 打开新的添加用户窗口
void UserManager::on_pushButton_clicked()
{
    addUser = new sys_add_user(this);
    addUser->exec();
    //    add->exec();
    model->select();
}

// 修改用户信息操作函数
void UserManager::on_pushButton_3_clicked()
{
    if (1 == ui->tableView->currentIndex().column()) {
        if (userCheck(ui->tableView->currentIndex().data(0).toString(),
                      SYS_USER_NUMBER)) {
            sqLite();
        } else {
            QMessageBox::warning(this, tr("提示"),
                                 "用户已存在，修改无效！");
            model->revertAll();
        }
    } else {
        sqLite();
    }

}

// 撤销数据库操作函数
void UserManager::on_pushButton_4_clicked()
{
    model->revertAll();
}

// 数据库操作函数
void UserManager::sqLite()
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

// 查询用户信息
void UserManager::on_pushButton_5_clicked()
{
    QString name = ui->lineEdit->text();

    if (NULL == name)
        QMessageBox::warning(this, tr("提示"),
                             "请输入用户名进行查询！");
    else if (!userCheck(name, SYS_USER_NUMBER)) {
        model->setFilter(QString("User_Name = '%1'")
                         .arg(name));
        model->select();
    } else {
        QMessageBox::warning(this, tr("提示"),
                             "用户不存在，请更换用户名重试！");
    }

    ui->lineEdit->clear();
}

// 重置操作
void UserManager::on_pushButton_6_clicked()
{
    tableModel();
}

// 初始化操作
void UserManager::tableModel()
{
    model = new QSqlTableModel(this);
    model->setTable("Sys_User");
    // 系统管理只能物流公司使用，即管理系统管理员
    model->setFilter("User_Is_Admin = 1");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("用户编号"));
    model->setHeaderData(1, Qt::Horizontal, tr("用户名"));
    model->setHeaderData(2, Qt::Horizontal, tr("用户密码"));
    model->setHeaderData(3, Qt::Horizontal, tr("用户邮箱"));
    model->setHeaderData(4, Qt::Horizontal, tr("用户身份"));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);
    // 不显示用户ID和身份，这些信息不必要
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(4);
    //    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}
