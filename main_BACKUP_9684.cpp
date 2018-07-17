#include "login.h"
#include "register.h"
#include "usermanager.h"
#include "staffmanager.h"
#include "sys_sqlite.h"
#include "mainwindow.h"
#include "staffmanager.h"

#include <QApplication>
#include <QTextCodec>
#include <QDebug>
#include <QDir>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//正确显示中文
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));//正确显示中文
<<<<<<< HEAD
    createConnectSqlite();
    QSqlQuery q;
    q.exec("delete from Storage_info");
    q.exec("delete from Storage_product");

    q.exec("insert into Sys_Seller values(0,'李','176','东')");
    q.exec("insert into Sys_Seller values(-1,'','','')");
    q.exec("select * from Storage_info");

    q.exec("insert into Storage_info values(1,'1号',100,100,0)");
    q.exec("insert into Storage_info values(2,'2号',100,100,-1)");
   // qDebug()<<QDir::currentPath();
   // qDebug()<<QDir::setCurrent(QString("../curly-tribble/img"));
   // qDebug()<<QDir::currentPath();

=======

	createConnectSqlite();
>>>>>>> 592fb2001b6fd976d931a2152b25d6661db2fecb

	Login login;
	login.show();

	return a.exec();
}
