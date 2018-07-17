#include "login.h"
#include "ui_login.h"

#include "sys_sqlite.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
//    mainw = new MainWindow(this);
//    QIcon *icon = new QIcon("/new/img/img/login/user.png");
//    QMessageBox mess;
//    mess.setWindowIcon(*icon);

//    QFile file(":/css/style/default.css");
//    file.open(QFile::ReadOnly);
//    QString styleSheet = tr(file.readAll());
//    this->setStyleSheet(styleSheet);
//    file.close();
    createConnectSqlite();

}

Login::~Login()
{
    delete ui;
}

void Login::reshow()
{
    this->show();
}

void Login::on_pushButton_clicked()
{
    QString login_name = ui->lineEdit->text();
    QString login_pwd = ui->lineEdit_2->text();
    QString admin = ui->comboBox->currentText();
    int identify;
    if (admin == "管理员")
        identify = 1;
    else {
        identify = 0;
        recordSeller(login_name);
    }
    if (NULL == login_name)
        QMessageBox::warning(this, "警告",
                             "用户名为空，请输入用户名！");
    if (NULL == login_pwd)
        QMessageBox::warning(this, "警告",
                             "密码为空，请输入密码！");

    QSqlTableModel model;
    model.setTable("Sys_User");
    model.setFilter(tr("User_Name = '%1' and User_Pwd = '%2' "
                       "and User_Is_Admin = '%3'")
                    .arg(login_name)
                    .arg(login_pwd)
                    .arg(identify));
    model.select();

    if (model.rowCount() == 1) {
        // 表示密码验证正确，登陆成功
        QMessageBox::information(this, "登录", "恭喜您登录成功！");

        //留着跳转到主窗口
        Data::is_admin = identify;
//        User::name = login_name;
        qDebug() << Data::is_admin;
        mainw = new MainWindow(this);
        mainw->show();
        this->hide();
    } else if (userCheck(login_name, SYS_USER_NUMBER)) {
        QMessageBox::warning(this, "警告",
                             "用户名不存在，请去注册！");
    } else if (!identityCheck(login_name, identify)) {
        QMessageBox::warning(this, "警告",
                             "身份错误，请确认身份！");
    } else if (!pwdCheck(login_name, login_pwd)) {
        QMessageBox::warning(this, "警告",
                             "密码错误，请重新输入！");
    }
}

void Login::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Login::on_pushButton_3_clicked()
{
//    Register *r = new Register;
    reg = new Register(this);
    reg->setWindowFlags(Qt::Window);
    this->hide();
    reg->show();
}

void Login::on_pushButton_4_clicked()
{
//    RecovePwd *rp = new RecovePwd;
    rp = new RecovePwd(this);
    rp->setWindowFlags(Qt::Window);
    rp->show();
}
