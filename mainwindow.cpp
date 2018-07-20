#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globaldata.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPixmap _image;
    _image.load(":/img/login/logo3.jpg");
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                            Qt::SmoothTransformation)));
    setPalette(pal);
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

void MainWindow::on_pushButton_clicked()
{
//    UserManager *um = new UserManager;
//    this->hide();
    um->show();
    um->activateWindow();
}

void MainWindow::on_pushButton_3_clicked()
{
//    StaffManager *sm = new StaffManager;
    sf->show();
    sf->activateWindow();
}

void MainWindow::on_pushButton_2_clicked()
{
//    StorageDialog *sd = new StorageDialog(this);
    sd->show();
    sd->activateWindow();
}

void MainWindow::on_pushButton_5_clicked()
{
//    Sale_Widget *sw = new Sale_Widget;
    sw->show();
    sw->activateWindow();
}

void MainWindow::on_pushButton_4_clicked()
{
//    stock_MainWindow *sm = new stock_MainWindow(this);
    sm->show();
    sm->activateWindow();
}

void MainWindow::on_pushButton_6_clicked()
{
//    this->close();
    this->parentWidget()->show();
    delete this;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    this->close();
    this->parentWidget()->show();
    delete this;
}
