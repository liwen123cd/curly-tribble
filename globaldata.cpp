/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：globaldata.cpp
  * @brief 内容: 初始化全局变量
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-10
  *
  * */
#include "globaldata.h"

// 初始化变量信息
int Data::is_admin = 1;
int User::id = 0;
QString User::name = NULL;
QString User::phone = NULL;
QString User::addr = NULL;
QString Staff::id = NULL;
QString Staff::name = NULL;
QString Staff::deperment = NULL;
QString Staff::position = NULL;
QString Staff::status = NULL;
QString Staff::phone = NULL;
QString Staff::date = NULL;
QString Staff::image = NULL;
QString Staff::description = NULL;
int Seller::id = 0;
QString Seller::name = NULL;
QString Seller::phone = NULL;
QString Seller::addr = NULL;
int Seller::number = 0;

