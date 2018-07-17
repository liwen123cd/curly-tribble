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
#include <QIcon>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));// 正确显示中文
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));// 正确显示中文

//    QIcon icon;
//    icon.addFile("/new/img/img/login/user.png");
////    icon.addPixmap();
//    QMessageBox::setIcon(icon);

	Login login;
	login.show();

	return a.exec();
}
