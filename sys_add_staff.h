/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：sys_add_staff.h
  * @brief 内容: 添加职工头文件
  * @date 作成日期: 2018-6-29
  * @date 修正日期：2018-7-16
  *
  * */
#ifndef SYS_ADD_STAFF_H
#define SYS_ADD_STAFF_H

#define STATUS_AT "在职"
#define STOCK_DEPENTMENT "仓储部"
#define FREIGHT_DEPENTMENT "货运部"

#include <QDialog>
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>

namespace Ui
{
class Sys_Add_Staff;
}

class Sys_Add_Staff : public QDialog
{
	Q_OBJECT

public:
	explicit Sys_Add_Staff(QWidget *parent = 0);
	~Sys_Add_Staff();

    void init();
private slots:
	void on_pushButton_2_clicked();

	void on_pushButton_clicked();

	void on_pushButton_3_clicked();

private:
	Ui::Sys_Add_Staff *ui;
	QString staff_filename;
};

#endif // SYS_ADD_STAFF_H
