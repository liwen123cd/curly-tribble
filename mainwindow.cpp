/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：MainWindow.cpp
  * @brief 内容: 主操作函数
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-19
  *
  * */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globaldata.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);

    // 添加背景图片
    QPixmap _image;
    _image.load(":/img/login/logo3.jpg");
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                                                        Qt::SmoothTransformation)));
    setPalette(pal);

    // 初始化整体布局
    QFile file(":/css/style/default.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = tr(file.readAll());
    this->setStyleSheet(styleSheet);
    file.close();
    if (0 == Data::is_admin) {
        ui->pushButton->setVisible(false);
        ui->pushButton_3->setVisible(false);
    }


    qDebug() << User::name;
    sf = new StaffManager(this);
    sf->setWindowFlags(Qt::Window);
    sf->setWindowModality(Qt::NonModal);
    um = new UserManager(this);
    um->setWindowFlags(Qt::Window);
    um->setWindowModality(Qt::NonModal);
    sw = new Sale_Widget(this);
    sw->setWindowFlags(Qt::Window);
    sw->setWindowModality(Qt::NonModal);
    sd = new StorageDialog(this);
    sm = new stock_MainWindow(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 打开用户管理子系统
void MainWindow::on_pushButton_clicked()
{
    um->show();
    um->activateWindow();
}

// 打开职工管理子系统
void MainWindow::on_pushButton_3_clicked()
{
    sf->show();
    sf->activateWindow();
}

// 打开仓库管理子系统
void MainWindow::on_pushButton_2_clicked()
{
    sd->show();
    sd->activateWindow();
}

// 打开销售管理子系统
void MainWindow::on_pushButton_5_clicked()
{
    sw->show();
    sw->activateWindow();
}

// 打开进货管理子系统
void MainWindow::on_pushButton_4_clicked()
{
    sm->show();
    sm->activateWindow();
}

// 退出系统
void MainWindow::on_pushButton_6_clicked()
{
    this->parentWidget()->show();
    delete this;
}

// 重写关闭窗口事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    this->close();
    this->parentWidget()->show();
    delete this;
}
