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

void createConnectSqlite();
bool isTableExist(QString tableName);
void createTable(QString createSql, QString tableName);
bool userCheck(QString inputText, int num);
bool pwdCheck(QString name, QString inputText);
bool identityCheck(QString name, int inputText);
void recordSeller(QString input);
void sqlOperator(QString sql);
bool checkExist(QString input, int number);
int Sale_Sql(const QString &sql);
bool sellerViewNull();
Seller infoSeller(int id);

#endif // SYS_SQLITE_H
