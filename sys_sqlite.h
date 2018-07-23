/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：sys_sqlite.h
  * @brief 内容: 数据库操作头文件
  * @date 作成日期: 2018-6-26
  * @date 修正日期：2018-7-15
  *
  * */
#ifndef SYS_SQLITE_H
#define SYS_SQLITE_H

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include "globaldata.h"

#define SYS_USER_NUMBER 1
#define SYS_STAFF_NUMBER 2
#define SYS_SELLER_NUMCER 3
#define STAFF_ID_NUMBER 1
#define STAFF_DEPENTMENT_NUMBER 2
#define STAFF_NAME_NUMBER 3
#define SELLER_ID_NUMBER 4
#define STORAGE_ID_NUMBER 5
#define STAFF_ID "员工ID"
#define STAFF_DEPENTMENT "部门"
#define STAFF_NAME "姓名"
#define SELLER_ID "卖家ID"
#define STORAGR_ID "仓库ID"

void createConnectSqlite(); // 连接数据库初始化函数
bool isTableExist(QString tableName); // 判断表是否存在
void createTable(QString createSql, QString tableName); // 创建表函数
bool userCheck(QString inputText, int num); // 用户检测合法性函数
bool pwdCheck(QString name, QString inputText); // 密码检测函数
bool identityCheck(QString name, int inputText); // 身份验证函数
void recordSeller(QString input); // 记录卖家信息函数
void sqlOperator(QString sql); // 数据库操作函数
bool checkExist(QString input, int number); // 检查表是否存在
int Sale_Sql(const QString &sql); // 数据库操作函数
bool sellerViewNull(); // 判断视图中是否存在数据
Seller infoSeller(int id); // 卖家信息函数

#endif // SYS_SQLITE_H
