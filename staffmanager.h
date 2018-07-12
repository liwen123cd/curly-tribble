#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRecord>
#include "globaldata.h"
#include "sys_add_staff.h"
#include "sys_maintain_staff.h"
#include "sys_add_seller.h"

namespace Ui
{
class StaffManager;
}

class StaffManager : public QWidget
{
    Q_OBJECT

public:
    explicit StaffManager(QWidget *parent = 0);
    ~StaffManager();

    void queryStaff(QString input);
    void init();
    void querySeller(QString inputText);
    void init2();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::StaffManager *ui;
    QSqlTableModel *model;
    QSqlTableModel *model2;
    Sys_Add_Staff *addStaff;
    Sys_Maintain_Staff *mainTain;
    Sys_Add_Seller *addSeller;
    Staff *staff;
};

#endif // STAFFMANAGER_H
