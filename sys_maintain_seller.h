#ifndef SYS_MAINTAIN_SELLER_H
#define SYS_MAINTAIN_SELLER_H

#include <QDialog>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlError>
#include <QErrorMessage>
#include <QSqlQuery>
#include <QString>

namespace Ui {
class Sys_Maintain_Seller;
}

class Sys_Maintain_Seller : public QDialog
{
    Q_OBJECT

public:
    explicit Sys_Maintain_Seller(QWidget *parent = 0);
    ~Sys_Maintain_Seller();

    void init();
    void ownQuery();
    void freeQuery();
    bool eventFilter(QObject *obj, QEvent *e);
private slots:
    void on_pushButton_del_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Sys_Maintain_Seller *ui;
    int sellerID;               // 上个界面传来的卖家ID
};

#endif // SYS_MAINTAIN_SELLER_H
