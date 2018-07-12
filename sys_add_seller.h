#ifndef SYS_ADD_SELLER_H
#define SYS_ADD_SELLER_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class Sys_Add_Seller;
}

class Sys_Add_Seller : public QDialog
{
    Q_OBJECT

public:
    explicit Sys_Add_Seller(QWidget *parent = 0);
    ~Sys_Add_Seller();

    void init();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Sys_Add_Seller *ui;
};

#endif // SYS_ADD_SELLER_H
