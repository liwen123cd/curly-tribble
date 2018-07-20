/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：register.h
  * @brief 内容: 注册头文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-19
  *
  * */
#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPixmap>
#include <QPalette>

namespace Ui
{
class Register;
}

class Register : public QWidget
{
	Q_OBJECT

public:
	explicit Register(QWidget *parent = 0);
	~Register();

	bool textCheck();
//    bool userCheck();
public slots:
	void closeEvent(QCloseEvent *event);

private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_comboBox_currentIndexChanged(int index);

private:
	Ui::Register *ui;
};

#endif // REGISTER_H
