/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：main.cpp
  * @brief 内容: 主操作函数
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-19
  *
  * */
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
#include <QMotifStyle>
#include <QMacStyle>
#include <QPlastiqueStyle>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));// 正确显示中文
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));// 正确显示中文

    // 设置整体风格
    QApplication::setStyle("macintosh");
    //    QApplication::setStyle(new QMotifStyle);
    QApplication::setStyle(new QPlastiqueStyle);

    Login login;
    login.show();

    return a.exec();
}
