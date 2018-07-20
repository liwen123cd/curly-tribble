/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：sys_add_user.h
  * @brief 内容: 添加用户头文件
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-11
  *
  * */
#ifndef SYS_ADD_USER_H
#define SYS_ADD_USER_H

#include <QDialog>
#include <QMessageBox>

namespace Ui
{
class sys_add_user;
}

class sys_add_user : public QDialog
{
	Q_OBJECT

public:
	explicit sys_add_user(QWidget *parent = 0);
	~sys_add_user();

private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

private:
	Ui::sys_add_user *ui;
};

#endif // SYS_ADD_USER_H
