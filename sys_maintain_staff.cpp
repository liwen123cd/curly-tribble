/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：Sys_Maintain_Staff.cpp
  * @brief 内容: 维护职工信息窗口
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-17
  *
  * */
#include "sys_maintain_staff.h"
#include "ui_sys_maintain_staff.h"
#include "globaldata.h"
#include "sys_sqlite.h"

Sys_Maintain_Staff::Sys_Maintain_Staff(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sys_Maintain_Staff)
{
    ui->setupUi(this);
    // 安装事件过滤器
    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
    init();
    if (NULL == description) {
        description = QString(Staff::description +
                              QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    } else {
        description += QString(Staff::description +
                               " " + QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    }

    // 标签自适应调整大小
    ui->label_15->adjustSize();
    ui->label_15->setWordWrap(true);
    ui->label_15->setAlignment(Qt::AlignTop);
}

Sys_Maintain_Staff::~Sys_Maintain_Staff()
{
    delete ui;
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
bool Sys_Maintain_Staff::eventFilter(QObject *obj, QEvent *e)
{
    // 先判断事件对象 再判断事件类型 重写操作处理函数
    if (obj == ui->lineEdit) {
        if (e->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
            if (mouseEvent->button() == Qt::LeftButton) {
                ui->lineEdit->setText(Staff::name);
            }
            return true;
        }
    } else if (obj == ui->lineEdit_2) {
        if (e->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
            if (mouseEvent->button() == Qt::LeftButton) {
                ui->lineEdit_2->setText(Staff::phone);
            }
            return true;
        }
    }
    // 返回系统默认的方法
    // 此处 Sys_MainTain_Staff 父类的方法
    return QDialog::eventFilter(obj, e);
}

// 重置操作
void Sys_Maintain_Staff::on_pushButton_2_clicked()
{
    init();
    //    ui->lineEdit->clear();
    //    ui->lineEdit_2->clear();
}

/**
  * @functionName Function Name : init()
  * @brief Description: 初始化操作函数
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Maintain_Staff::init()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->label_9->setText(Staff::name);
    ui->label_10->setText(Staff::deperment);
    ui->label_11->setText(Staff::position);
    ui->label_12->setText(Staff::status);
    ui->label_13->setText(Staff::date);
    ui->label_14->setText(Staff::phone);
    if (NULL == Staff::description)
        ui->label_15->setText(TEXTNULL);
    else
        ui->label_15->setText(Staff::description);
    ui->lineEdit->setPlaceholderText(Staff::name);
    ui->lineEdit_2->setPlaceholderText(Staff::phone);
    //    ui->comboBox->setEditText(Staff::deperment);
    //    ui->comboBox_2->setEditText(Staff::position);
    if (DEPENTMENT_SHOPPING == Staff::deperment)
        ui->comboBox->setCurrentIndex(SHOPPING);
    if (DEPENTMENT_WAREHOUSE == Staff::deperment)
        ui->comboBox->setCurrentIndex(WAREHOUSE);
    if (POSITION_COMMON == Staff::position)
        ui->comboBox_2->setCurrentIndex(COMMONSTAFF);
    if (POSITION_MANAGER == Staff::position)
        ui->comboBox_2->setCurrentIndex(MANAGER);
    if (POSITION_SECRETARY == Staff::position)
        ui->comboBox_2->setCurrentIndex(SECRETARY);
    ui->label_21->setText(Staff::status);
    ui->label_22->setVisible(false);
}

/**
  * @functionName Function Name : statusChange()
  * @brief Description: 员工状态改变函数
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Maintain_Staff::statusChange()
{
    // 如果职工换岗 变换职位为换岗
    if (Staff::deperment != ui->comboBox->currentText()) {
        ui->label_21->setText(STATUS_CHANGE);
        qDebug() << description;
        description += QString(tr(":从") + Staff::deperment + tr("到") +
                               ui->comboBox->currentText() + " ");
        ui->label_15->setText(description);
    } else {
        // 如果没有职位变动保持不变
        ui->label_21->setText(STATUS_AT);
    }
}

/**
  * @functionName Function Name : on_pushButton_clicked()
  * @brief Description: 更新职工信息
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Maintain_Staff::on_pushButton_clicked()
{
    // 判断输入合法性
    if (NULL == ui->lineEdit->text())
        QMessageBox::warning(this, tr("提示"),
                             "请填写职工姓名！");
    if (NULL == ui->lineEdit_2->text())
        QMessageBox::warning(this, tr("提示"),
                             "请填写职工电话！");
    if (NULL != ui->lineEdit->text()
            && NULL != ui->lineEdit_2->text()) {
        int ok = QMessageBox::warning(this, tr("确定修改职工？"),
                                      tr("你确定修改当前职工吗？"),
                                      QMessageBox::Yes,
                                      QMessageBox::No);
        if (ok == QMessageBox::Yes) {

            // 判断职位和部门是否改变
            statusChange();
            positionChange();

            QSqlQuery query;
            QString updateSql = QString("update Sys_Staff "
                                        "set Staff_Name = :Staff_Name, "
                                        "Staff_Deperment = :Staff_Deperment, "
                                        "Staff_Position = :Staff_Position, "
                                        "Staff_Status = :Staff_Status, "
                                        "Staff_Phone = :Staff_Phone, "
                                        "Staff_Description = :Staff_Description "
                                        "where Staff_Id = :Staff_Id");
            query.prepare(updateSql);
            query.bindValue(":Staff_Name", ui->lineEdit->text());
            query.bindValue(":Staff_Deperment", ui->comboBox->currentText());
            query.bindValue(":Staff_Position", ui->comboBox_2->currentText());
            query.bindValue(":Staff_Status", ui->label_21->text());
            query.bindValue(":Staff_Phone", ui->lineEdit_2->text());
            query.bindValue(":Staff_Description", ui->label_15->text());
            query.bindValue(":Staff_Id", Staff::id);

            qDebug() << ui->label_15->text();

            if (!query.exec()) {
                QMessageBox::information(this, "提示",
                                         query.lastError().text());
                qDebug() << query.lastError();

            } else {
                QMessageBox::information(this, "提示",
                                         "您已更新职工信息！");
            }
        } else {
            init();
            this->close();
        }
    }
}

/**
  * @functionName Function Name : positionChange()
  * @brief Description: 判断职工职位是否改变
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Maintain_Staff::positionChange()
{
    ui->label_22->setVisible(false);
    qDebug() << Staff::position;
    qDebug() << ui->comboBox_2->currentText();
    if (Staff::position != ui->comboBox_2->currentText()) {
        ui->label_22->setVisible(true);
        // 对各种职位变动设置相应的备注和日期记录
        if (Staff::position == POSITION_COMMON
                && ui->comboBox_2->currentText() == POSITION_MANAGER) {
            ui->label_22->setText(POSITION_PROMOTE);
            description += QString(tr("从") + POSITION_COMMON +
                                   tr("到") + POSITION_MANAGER + " ");
            ui->label_15->setText(description);
        }
        if (Staff::position == POSITION_COMMON
                && ui->comboBox_2->currentText() == POSITION_SECRETARY) {
            ui->label_22->setText(POSITION_PROMOTE);
            description += QString(tr("从") + POSITION_COMMON +
                                   tr("到") + POSITION_SECRETARY + " ");
            ui->label_15->setText(description);
        }
        if (Staff::position == POSITION_SECRETARY
                && ui->comboBox_2->currentText() == POSITION_MANAGER) {
            ui->label_22->setText(POSITION_PROMOTE);
            description += QString(tr("从") + POSITION_SECRETARY +
                                   tr("到") + POSITION_MANAGER + " ");
            ui->label_15->setText(description);
        }
        if (Staff::position == POSITION_SECRETARY
                && ui->comboBox_2->currentText() == POSITION_COMMON) {
            ui->label_22->setText(POSITION_DOWN);
            description += QString(tr("从") + POSITION_SECRETARY +
                                   tr("到") + POSITION_COMMON + " ");
            ui->label_15->setText(description);
        }
        if (Staff::position == POSITION_MANAGER
                && ui->comboBox_2->currentText() == POSITION_COMMON) {
            ui->label_22->setText(POSITION_DOWN);
            description += QString(tr("从") + POSITION_MANAGER +
                                   tr("到") + POSITION_COMMON + " ");
            ui->label_15->setText(description);
        }
        if (Staff::position == POSITION_MANAGER
                && ui->comboBox_2->currentText() == POSITION_SECRETARY) {
            ui->label_22->setText(POSITION_DOWN);
            description += QString(tr("从") + POSITION_MANAGER +
                                   tr("到") + POSITION_SECRETARY + " ");
            ui->label_15->setText(description);
        }
    }
}

/**
  * @functionName Function Name : on_pushButton_3_clicked()
  * @brief Description: 删除职工并不是将职工真正删除
  *                     而是将职工的久之状态改为离职
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Maintain_Staff::on_pushButton_3_clicked()
{
    int ok = QMessageBox::warning(this, tr("确定删除职工？"),
                                  tr("你确定删除当前职工吗？"),
                                  QMessageBox::Yes,
                                  QMessageBox::No);
    if (ok == QMessageBox::Yes) {

        // 更新职工信息并改变相应状态
        QSqlQuery query;
        QString updateSql = QString("update Sys_Staff "
                                    "set Staff_Status = :Staff_Status, "
                                    "Staff_Description = :Staff_Description "
                                    "where Staff_Id = :Staff_Id");
        query.prepare(updateSql);
        qDebug() << description + STATUS_LEAVE;
        query.bindValue(":Staff_Status", STATUS_LEAVE);
        query.bindValue(":Staff_Description", description + STATUS_LEAVE + " ");
        query.bindValue(":Staff_Id", Staff::id);
        qDebug() << description;
        if (!query.exec()) {
            QMessageBox::information(this, "提示",
                                     query.lastError().text());
        } else {
            QMessageBox::information(this, "提示",
                                     "您已将该职工删除！");
        }
        qDebug() << updateSql;
        this->close();
    }
}
