/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：mainwindow.h
  * @brief 内容: 主窗口
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-19
  *
  * */
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

public slots:
    void closeEvent(QCloseEvent *event);
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
