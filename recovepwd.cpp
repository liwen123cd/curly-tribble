/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：RecovePwd.cpp
  * @brief 内容: 找回密码 并写了补全功能
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-10
  *
  * */
#include "recovepwd.h"
#include "ui_recovepwd.h"

RecovePwd::RecovePwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecovePwd)
{
    ui->setupUi(this);

    // 设置背景图片
    QPixmap _image;
    _image.load(":/img/login/log2.jpg");
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                                                        Qt::SmoothTransformation)));
    setPalette(pal);

    // 设置 LineEdit 自动提示补全
    m_model = new QStandardItemModel(0, 1, this);
    QCompleter *m_completer = new QCompleter(m_model, this);
    ui->lineEdit->setCompleter(m_completer);

    connect(m_completer, SIGNAL(activated(const QString&)),
            this, SLOT(onEmailChoosed(const QString&)));
    connect(ui->lineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(onTextChanged(const QString&)));
}

RecovePwd::~RecovePwd()
{
    delete ui;
}

// 完成找回密码功能
void RecovePwd::on_pushButton_clicked()
{
    QString email = ui->lineEdit->text();
    if (email.length() == 0) {
        QMessageBox::information(this, "提示", "未填写邮箱！");
    } else {
        QMessageBox::information(this, "提示", "发送成功！"
                                             "请按提示步骤找回密码！");
        this->close();
    }
}

// 重置操作
void RecovePwd::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
}

// 清除已存在的文本更新内容
void RecovePwd::onEmailChoosed(const QString& email)
{
    ui->lineEdit->clear();
    ui->lineEdit->setText(email);
}

/**
  * @functionName Function Name : onTextChange(const QString& str)
  * @brief Description: QlineEdit自动补全邮箱功能
  * @date Date: 2018-7-4
  * @param Parameter: str
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void RecovePwd::onTextChanged(const QString& str)
{
    // 如果已经输入了@符号，我们就停止补全。
    // 因为到了这一步，我们再补全意义也不大了。
    if (str.contains("@")) {
        return;
    }
    QStringList strlist;
    strlist << "@163.com" << "@qq.com" << "@gmail.com" << "@hotmail.com" << "@126.com";
    // 先清楚已经存在的数据，不然的话每次文本
    // 变更都会插入数据，最后出现重复数据
    m_model->removeRows(0, m_model->rowCount());
    for (int i = 0; i < strlist.size(); ++i) {
        m_model->insertRow(0);
        m_model->setData(m_model->index(0, 0), str + strlist.at(i));
    }
}
