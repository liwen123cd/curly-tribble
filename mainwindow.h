#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "usermanager.h"
#include "staffmanager.h"
#include "storagedialog.h"
#include "sale_widget.h"
#include "stock_mainwindow.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    stock_MainWindow*sm;
    StaffManager *sf;
    UserManager *um;
    Sale_Widget *sw;
    StorageDialog *sd;
};

#endif // MAINWINDOW_H
