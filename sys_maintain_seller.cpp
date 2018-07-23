/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：Sys_Maintain_Seller.cpp
  * @brief 内容: 维护职工信息源文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-17
  *
  * */
#include "sys_maintain_seller.h"
#include "ui_sys_maintain_seller.h"
#include "globaldata.h"

Sys_Maintain_Seller::Sys_Maintain_Seller(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sys_Maintain_Seller)
{
    ui->setupUi(this);
    // 从上一个界面获取到的卖家ID
    sellerID = Seller::id;

    init();
    ui->lineEdit->clearFocus();

    // 为三个 QLineEdit 安装事件过滤器
//    ui->lineEdit->setCursor(Qt::BlankCursor);
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit_3->installEventFilter(this);

}

Sys_Maintain_Seller::~Sys_Maintain_Seller()
{
    delete ui;
}

/**
  * @functionName Function Name : init()
  * @brief Description: 初始化操作
  * @date Date: 2018-7-10
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Maintain_Seller::init()
{
    // 界面展示职工信息
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->label_5->setText(Seller::name);
    ui->label_6->setText(Seller::phone);
    ui->label_7->setText(Seller::addr);
    ui->lineEdit->setPlaceholderText(Seller::name);
    ui->lineEdit_2->setPlaceholderText(Seller::phone);
    ui->lineEdit_3->setPlaceholderText(Seller::addr);

    QString stofree;
    QString stoPro;
    QSqlQuery query;

    qDebug()<<sellerID;
    qDebug()<<"sas";
    // 查询空闲仓库
    query.exec("select storageName from Storage_info "
               "where sellerID = -1");
    while (query.next()) {
        stofree += query.value(0).toString() + "\n";
    }
    query.exec(QString("select storageName from Storage_info "
                       "where sellerID=%1").arg(QString::number(sellerID)));

    // 查询拥有仓库
    while (query.next()) {
        stoPro += query.value(0).toString() + "\n";
    }

    // 查询仓库数量
    query.exec(QString("select count(*) from Storage_info "
               "where sellerID = '%1'").arg(QString::number(sellerID)));
    while(query.next()) {
        Seller::number = query.value(0).toInt();
    }

    // 显示
    ui->label_8->setNum(Seller::number);
    ui->textBrowser_pro->setText(stoPro);
    ui->textBrowser_free->setText(stofree);
}

// 删除仓库槽函数
void Sys_Maintain_Seller::on_pushButton_del_clicked()
{
    QString target = ui->lineEdit_pro->text();
    if (NULL == target) {
        QMessageBox::information(this, tr("提示"),
                                 "请输入需要删除的仓库！");
    } else {
        QSqlQuery query;
        query.exec(QString("update Storage_info "
                           "set sellerID=-1 "
                           "where storageName='%1'").arg(target));
        if (query.lastError().isValid()) {
            qDebug() << query.lastError().text();
        } else {
            QMessageBox::information(this, tr("提示"),
                                     "您已成功删除仓库！");
        }
        init();
    }
}

// 添加仓库槽函数
void Sys_Maintain_Seller::on_pushButton_add_clicked()
{
    QString target = ui->lineEdit_free->text();
    if (NULL == target) {
        QMessageBox::information(this, tr("提示"),
                                 "请输入需要添加的仓库！");
    } else {
        QSqlQuery query;
        query.exec(QString("update Storage_info "
                           "set sellerID=%1 "
                           "where storageName='%2'")
                   .arg(sellerID)
                   .arg(target));
        if (query.lastError().isValid()) {
            qDebug() << query.lastError().text();
        } else {
            QMessageBox::information(this, tr("提示"),
                                     "您已成功添加仓库！");
        }
        init();
    }
}

/**
  * @functionName Function Name : on_pushButton_clicked()
  * @brief Description: 更新买家信息主操作
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Maintain_Seller::on_pushButton_clicked()
{
    // 检查输入合法性
    if (NULL == ui->lineEdit->text()) {
        QMessageBox::warning(this, tr("提示"),
                             "请填写卖家姓名！");
    }
    if (NULL == ui->lineEdit_2->text()) {
        QMessageBox::warning(this, tr("提示"),
                             "请填写卖家电话！");
    }
    if (NULL == ui->lineEdit_3->text()) {
        QMessageBox::warning(this, tr("提示"),
                             "请填写卖家地址！");
    }
    if (NULL != ui->lineEdit->text()
            && NULL != ui->lineEdit_2->text()) {
        int ok = QMessageBox::warning(this, tr("确定修改卖家？"),
                                      tr("你确定修改当前卖家信息吗？"),
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if (ok == QMessageBox::Yes) {

            // 更新卖家信息
            QSqlQuery query;
            QString updateSql = QString("update Sys_Seller "
                                        "set Seller_Name = :Seller_Name, "
                                        "Seller_Phone = :Seller_Phone, "
                                        "Seller_Addr = :Seller_Addr "
                                        "where Seller_Id = :Seller_Id");
            query.prepare(updateSql);
            query.bindValue(":Seller_Name", ui->lineEdit->text());
            query.bindValue(":Seller_Phone", ui->lineEdit_2->text());
            query.bindValue(":Seller_Addr", ui->lineEdit_3->text());
            query.bindValue(":Seller_Id", Seller::id);

            if (!query.exec()) {
                QMessageBox::information(this, "提示",
                                         query.lastError().text());
                qDebug() << query.lastError();

            } else {
                QMessageBox::information(this, "提示",
                                         "您已更新卖家信息！");
                this->close();
            }
        } else {
            init();
            this->close();
        }
    }
}

/**
  * @functionName Function Name : eventFilter(QObject *obj, QEvent *e)
  * @brief Description: 事件过滤器实现当鼠标点击LineEdit时
  *                     将默认的背景文字填充到文本框内
  * @date Date: 2018-7-4
  * @param Parameter: obj e
  * @return Return Code: true
  * @author Author: 张岩森
  *
  * */
bool Sys_Maintain_Seller::eventFilter(QObject *obj, QEvent *e)
{
    // 先判断发生事件的控件对象
    if (obj == ui->lineEdit) {
        // 判断事件的类型
        if (e->type() == QEvent::MouseButtonPress) {
            // 判断是否为鼠标左击事件
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
            if (mouseEvent->button() == Qt::LeftButton) {
                // 将默认数值填充
                ui->lineEdit->setText(Seller::name);
            }
            return true;
        }
    } else if (obj == ui->lineEdit_2) {
        if (e->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
            if (mouseEvent->button() == Qt::LeftButton) {
                ui->lineEdit_2->setText(Seller::phone);
            }
            return true;
        }
    } else if (obj == ui->lineEdit_3 ) {
        if (e->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
            if (mouseEvent->button() == Qt::LeftButton) {
                ui->lineEdit_3->setText(Seller::addr);
            }
            return true;
        }
    }
    // 返回系统默认的方法
    // 此处 Sys_MainTain_Seller 父类的方法
    return QDialog::eventFilter(obj, e);
}

// 初始化操作
void Sys_Maintain_Seller::on_pushButton_2_clicked()
{
    init();
}
