#include "sys_add_staff.h"
#include "ui_sys_add_staff.h"
#include "sys_sqlite.h"

Sys_Add_Staff::Sys_Add_Staff(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sys_Add_Staff)
{
    ui->setupUi(this);
    staff_filename = ":/img/img/na.jpg";
    ui->radioButton->setChecked(true);
    init();
}

Sys_Add_Staff::~Sys_Add_Staff()
{
    delete ui;
}

/**
  * @functionName Function Name : on_pushButton_2_clicked()
  * @brief Description: 添加职工操作函数 默认职工在职
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void Sys_Add_Staff::on_pushButton_2_clicked()
{
    int number;
    QString staff_name = ui->lineEdit->text();
    QString staff_deperment;
    if (ui->radioButton->isChecked()) {
        staff_deperment = ui->radioButton->text();
    }
    if (ui->radioButton_2->isChecked()) {
        staff_deperment = ui->radioButton_2->text();
    }
    QString staff_position = ui->comboBox->currentText();
    QString staff_phone = ui->lineEdit_2->text();
    QString staff_description = ui->textEdit->toPlainText();
    QString staff_status = STATUS_AT;
    QString staff_date = QString(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    qDebug() << staff_date;
    QString staff_date2 = QString(QDateTime::currentDateTime().toString("yyyyMMdd"));
    qDebug() << staff_date2;
    QString staff_id = NULL;

    // 从数据库中读取职工的数量
    QSqlQuery query;
    query.exec("select count(*) from Sys_Staff");
    while (query.next()) {
        number = query.value(0).toInt();
    }
    if (STOCK_DEPENTMENT == staff_deperment)
        staff_id = "SD" + staff_date2 + QString("%1").arg(number, 5, 10, QLatin1Char('0'));
    else if (FREIGHT_DEPENTMENT == staff_deperment)
        staff_id = "FD" + staff_date2 + QString("%1").arg(number, 5, 10, QLatin1Char('0'));
    qDebug() << staff_id;

    if (NULL == staff_name)
        QMessageBox::warning(this, tr("提示"),
                             "请填写职工姓名！");
    if (NULL == staff_phone)
        QMessageBox::warning(this, tr("提示"),
                             "请填写职工电话！");
    if (NULL != staff_name && NULL != staff_phone) {
        if (!userCheck(staff_name, SYS_STAFF_NUMBER)) {
            QMessageBox::warning(this, tr("提示"),
                                 "职工已存在，请确认职工姓名！");
        } else {
            QString add_sql = QString("insert into Sys_Staff(Staff_Id, "
                                      "Staff_Name, Staff_Deperment, "
                                      "Staff_Position, Staff_Status, "
                                      "Staff_Phone, Staff_Date, "
                                      "Staff_Image, Staff_Description)");
            add_sql += QString(" VALUES('%1','%2','%3','%4',"
                               "'%5', '%6', '%7', '%8', '%9')")
                    .arg(staff_id)
                    .arg(staff_name)
                    .arg(staff_deperment)
                    .arg(staff_position)
                    .arg(staff_status)
                    .arg(staff_phone)
                    .arg(staff_date)
                    .arg(staff_filename)
                    .arg(staff_description);

            QSqlQuery query;
            query.exec(add_sql);

            QMessageBox::information(this, "添加成功",
                                     "您已成功添加新职工！");
            init();
            this->close();
        }
    }

}

// 为职工选择图片
void Sys_Add_Staff::on_pushButton_clicked()
{
    staff_filename = QFileDialog::getOpenFileName(this, tr("选择图片"),
                                                  "", tr("Images(*.png *.jpg *.jpeg)"));
    if (QString::compare(staff_filename, QString()) != 0) {
        QImage image;
        bool valid = image.load(staff_filename);
        if (valid) {
            image = image.scaledToWidth(ui->label_7->width(), Qt::SmoothTransformation);
            ui->label_7->setPixmap(QPixmap::fromImage(image));
        }
    }
    qDebug() << staff_filename;
}

// 重置操作
void Sys_Add_Staff::on_pushButton_3_clicked()
{
    init();
}

// 初始化操作
void Sys_Add_Staff::init()
{
    ui->radioButton->setChecked(true);
    ui->comboBox->setCurrentIndex(0);
    ui->textEdit->clear();
    staff_filename = ":/img/img/na.jpg";
    ui->label_7->setPixmap(QPixmap(staff_filename));
}
