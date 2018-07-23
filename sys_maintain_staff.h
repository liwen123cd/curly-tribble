/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：sys_maintain_staff.h
  * @brief 内容: 维护职工信息头文件
  * @date 作成日期: 2018-6-31
  * @date 修正日期：2018-7-18
  *
  * */
#ifndef SYS_MAINTAIN_STAFF_H
#define SYS_MAINTAIN_STAFF_H

#define COMMONSTAFF 0
#define MANAGER 1
#define SECRETARY 2

#define SHOPPING 0
#define WAREHOUSE 1

#define DEPENTMENT_SHOPPING "货运部"
#define DEPENTMENT_WAREHOUSE "仓储部"
#define POSITION_COMMON "普通职工"
#define POSITION_MANAGER "部门经理"
#define POSITION_SECRETARY "经理秘书"
#define STATUS_AT "在职"
#define STATUS_CHANGE "换岗"
#define STATUS_LEAVE "离职"
#define POSITION_PROMOTE "升迁"
#define POSITION_DOWN "降职"
#define TEXTNULL "无"

#include <QDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlError>
#include <QErrorMessage>

namespace Ui
{
class Sys_Maintain_Staff;
}

class Sys_Maintain_Staff : public QDialog
{
    Q_OBJECT

public:
    explicit Sys_Maintain_Staff(QWidget *parent = 0);
    ~Sys_Maintain_Staff();

    bool eventFilter(QObject *obj, QEvent *e);

    void init();
    void statusChange();
    void positionChange();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Sys_Maintain_Staff *ui;
    QString description;
};

#endif // SYS_MAINTAIN_STAFF_H
