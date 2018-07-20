/**
  * @author 著作权所有者: 张岩森
  * @projectName 文件名：sys_sqlite.cpp
  * @brief 内容: 数据库操作处理函数
  * @date 作成日期: 2018-6-28
  * @date 修正日期：2018-7-17
  *
  * */
#include "sys_sqlite.h"
#include "globaldata.h"

/**
  * @functionName Function Name : createConnectSqlite()
  * @brief Description: 连接数据库函数并初始化各个表
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void createConnectSqlite()
{
    // 查看数据库中可用的驱动
    qDebug() << "可用的驱动：";
    QStringList drivers = QSqlDatabase::drivers();
    foreach (QString driver, drivers) {
        qDebug() << driver;
    }

    // 创建连接，使用默认的SqLite
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");
    qDebug() << db.databaseName();
    bool ok = db.open();
    if (ok)
        qDebug() << "数据库连接成功";
    else
        qDebug() << "数据库连接失败";

    // 创建数据库表 表名
    QString userTable = "Sys_User";
    QString staffTable = "Sys_Staff";
    QString sellerTable = "Sys_Seller";
    QString saleOrderTable = "Sale_Order";
    QString saleStateTable = "Sale_State";
    QString storageInfoTable = "Storage_info";
    QString storageProductTable = "Storage_product";
    QString storageWaitProduct = "Storage_wait_product";
    QString storageOrderRecordTable = "Storage_order_record";
    QString mainView = "Main_view";
    QString sellerView = "Seller_View";
    QString stockPlanTable = "stock_plan";
    QString stockProviderTable = "stock_provider";
    QString stockProviderProductTable = "stock_provider_product";
    QString stockPlanDetailTable = "stock_plan_detail";
    QString stockCanceledplanTable = "stock_canceledplan";
    QString stockCanceledplanDetailTable = "stock_canceledplan_detail";

    // 建表语句
    // 用户信息表
    QString createUserSql = "create table Sys_User"
                            "("
                            "User_Id integer primary key, "
                            "User_Name varchar(20), "
                            "User_Pwd varchar(20), "
                            "User_Email varchar(20), "
                            "User_Is_Admin int"
                            ")";

    // 职工信息表
    QString createStaffSql = "create table Sys_Staff"
                             "("
                             "Staff_Id varchar(10) primary key, "
                             "Staff_Name varchar(20), "
                             "Staff_Deperment varchar(20), "
                             "Staff_Position varchar(20), "
                             "Staff_Status varchar(20), "
                             "Staff_Phone varchar(20), "
                             "Staff_Date varchar(30), "
                             "Staff_Image varchar(50), "
                             "Staff_Description varchar(50)"
                             ")";

    // 卖家信息表
    QString createSellerSql = "create table Sys_Seller"
                              "("
                              "Seller_Id integer primary key, "
                              "Seller_Name varchar(20), "
                              "Seller_Phone varchar(20), "
                              "Seller_Addr varchar(30)"
                              ")";

    // 销售订单信息表
    QString createSaleOrderSql = "create table Sale_Order("
                                 "Sale_Order_ID varchar(30) primary key,"
                                 "Sale_Buyer_Name varchar(20),"
                                 "Sale_Buyer_Tel varchar(20),"
                                 "Sale_Buyer_Address varchar(40),"
                                 "Sale_Seller_ID integer "
                                 "REFERENCES Sys_Seller (Seller_Id)," //外键
            "Sale_Item_ID integer "
            "REFERENCES stock_provider_product (id),"//外键
            "Sale_Item_Num integer,"
            "Sale_Item_Price float,"
            "Sale_Order_Finished integer)";

    // 订单状态信息表
    QString createSaleStateSql = "create table Sale_State("
                                 "Sale_State_ID integer primary key autoincrement,"
                                 "Sale_Order_ID varchar(30),"//外键
            "Sale_Order_State varchar(20),"
            "Sale_Date datetime,"
            "foreign key (Sale_Order_ID) "
            "references Sale_Order(Sale_Order_ID) on delete cascade)";
    // 外键约束
    QString forignSql = "PRAGMA foreign_keys = ON";

    // 仓库信息表
    static QString createStorageInfoSql = "create table Storage_info("
                                          "storageID integer,"
                                          "storageName varchar(10),"
                                          "capacity integer,"
                                          "remain integer,"
                                          "sellerID integer,"
                                          "primary key(storageID),"
                                          "foreign key(sellerID) references Sys_Seller(Seller_Id)"
                                          ")";

    // 库存商品信息表
    static QString createStorageProductSql = "create table Storage_product("
                                             "storageID integer,"
                                             "productID integer,"
                                             "amount integer,"
                                             "primary key(storageID,productID),"
                                             "foreign key(storageID) references Storage_info(storageID),"
                                             "foreign key(productID) references stock_provider_product(id)"
                                             ")";

    // 待出库记录表
    static QString createWaitProduct = "create table Storage_wait_product("
                                       "orderID varchar(30),"
                                       "sellerID integer,"
                                       "productID integer,"
                                       "amount integer,"
                                       "primary key(orderID)"
                                       ")";

    // 出库记录表
    static QString createOrderRecordSql = "create table Storage_order_record("
                                          "orderID varchar(30),"
                                          "productID integer,"
                                          "orderDate datetime,"
                                          "amount integer,"
                                          "primary key(orderID),"
                                          "foreign key(productID) references stock_provider_product(id)"
                                          ")";

    // 界面显示view
    static QString mainViewSql = "create view Main_view as "
                                 "select si.storageName, s.Seller_Name, p.name, sp.amount "
                                 "from Sys_Seller s, stock_provider_product p, Storage_info si, Storage_product sp "
                                 "where si.sellerID = s.Seller_Id "
                                 "and sp.storageID = si.storageID "
                                 "and sp.productID = p.id";

    // 卖家信息管理显示 view
    static QString sellerViewSql = "create view Seller_View as "
                                   "select s.Seller_Id, s.Seller_Name, "
                                   "s.Seller_Phone, s.Seller_Addr, "
                                   "si.storageID, si.storageName, si.remain "
                                   "from Sys_Seller s "
                                   "left join Storage_info si "
                                   "on s.Seller_Id = si.sellerID";

    // 进货计划信息表
    QString stockPlanSql = "create table stock_plan("
                           "id int primary key, "
                           "seller_id int, "
                           "money float, "
                           "fee float, "
                           "time Date)";

    // 进货商信息表
    QString stockProviderSql = "create table stock_provider("
                               "id int primary key, "
                               "owner_id int, "
                               "name varchar, "
                               "address varchar)";

    // 进货商商品信息表
    QString stockProviderProductSql = "create table stock_provider_product("
                                      "id int primary key, "
                                      "provider_id int, "
                                      "name varchar, "
                                      "price float,"
                                      "path varchar(50))";

    // 进货详细计划信息表
    QString stockPlanDetailSql = "create table stock_plan_detail("
                                 "id int primary key, "
                                 "plan_id int, "
                                 "product_id int, "
                                 "cnt int, "
                                 "price float, "
                                 "state varchar)";

    // 进货取消信息表
    QString stockCanceledplanSql = "create table stock_canceledplan("
                                   "id int primary key, "
                                   "seller_id int, "
                                   "money float, "
                                   "time Date)";

    // 取消进货详细计划表
    QString stockCanceledplanDetailSql = "create table stock_canceledplan_detail("
                                         "id int primary key, "
                                         "plan_id int, "
                                         "product_id int, "
                                         "cnt int, "
                                         "price float)";

    createTable(createUserSql, userTable);
    createTable(createStaffSql, staffTable);
    createTable(createSellerSql, sellerTable);
    createTable(createSaleOrderSql, saleOrderTable);
    createTable(createSaleStateSql, saleStateTable);
    createTable(createStorageInfoSql, storageInfoTable);
    createTable(createStorageProductSql, storageProductTable);
    createTable(createOrderRecordSql, storageOrderRecordTable);
    createTable(createWaitProduct, storageWaitProduct);
    createTable(stockPlanSql, stockPlanTable);
    createTable(stockProviderProductSql, stockProviderProductTable);
    createTable(stockPlanDetailSql, stockPlanDetailTable);
    createTable(stockCanceledplanDetailSql, stockCanceledplanDetailTable);
    createTable(stockCanceledplanSql, stockCanceledplanTable);
    createTable(stockProviderSql, stockProviderTable);
    createTable(mainViewSql, mainView);
    createTable(sellerViewSql, sellerView);
    Sale_Sql(forignSql);
}

/**
  * @functionName Function Name : createTable(QString createSql, QString tableName)
  * @brief Description: 创建数据库表的函数
  * @date Date: 2018-7-4
  * @param Parameter: createSql 创表语句 tableName 表名
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void createTable(QString createSql, QString tableName)
{
    QSqlQuery query;
    query.exec(createSql);
    if (isTableExist(tableName)) {
        qDebug() << tableName + "表已经存在，无须重新创建！";
    } else {
        if (!query.exec(createSql)) {
            qDebug() << "创建" + tableName + "表失败！";
            qDebug() << query.lastError();
        } else {
            qDebug() << "创建" + tableName + "表成功！";
        }
    }

}

/**
  * @functionName Function Name : isTableExist(QString tableName)
  * @brief Description: 判断要创建的数据库表是否存在
  * @date Date: 2018-7-4
  * @param Parameter: tableName 表名
  * @return Return Code: true
  * @author Author: 张岩森
  *
  * */
bool isTableExist(QString tableName)
{
    QSqlDatabase database;
    QSqlQuery query(database);
    bool isTableExist = query.exec(
                QString(
                    "select count(*) from sqlite_master "
                    "where type='table' and name='%1'").arg(tableName));
    return isTableExist;
}

/**
  * @functionName Function Name : userCheck(QString inputText, int num)
  * @brief Description: 判断用户是否存在
  * @date Date: 2018-7-4
  * @param Parameter: inputText 要判断的数据 num 表的代号
  * @return Return Code: true
  * @author Author: 张岩森
  *
  * */
bool userCheck(QString inputText, int num)
{
    bool exists = true;

    QSqlQuery query;
    QString checkName;
    switch (num) {
    case SYS_USER_NUMBER:
        checkName = QString("select ifnull("
                            "(select User_Name "
                            "from Sys_User "
                            "where User_Name = '%1' "
                            "limit 1), "
                            "0)")
                .arg(inputText);
        break;
    case SYS_STAFF_NUMBER:
        checkName = QString("select ifnull("
                            "(select Staff_Name "
                            "from Sys_Staff "
                            "where Staff_Name = '%1' "
                            "limit 1), "
                            "0)")
                .arg(inputText);
        break;
    case SYS_SELLER_NUMCER:
        checkName = QString("select ifnull("
                            "(select Seller_Name "
                            "from Sys_Seller "
                            "where Seller_Name = '%1' "
                            "limit 1), "
                            "0)")
                .arg(inputText);
        break;
    default:
        break;
    }

    query.exec(checkName);

    while (query.next()) {
        if ("0" == query.value(0).toString())
            exists = true;
        else
            exists = false;
    }
    return exists;
}

/**
  * @functionName Function Name : pwdCheck(QString name, QString inputText)
  * @brief Description: 判断用户密码是否正确
  * @date Date: 2018-7-4
  * @param Parameter: name 用户名 inputText 要判断的数据
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
bool pwdCheck(QString name, QString inputText)
{
    bool flag = false;

    QSqlQuery query;
    QString checkName = QString("(select User_Pwd "
                                "from Sys_User "
                                "where User_Name = '%1' "
                                "limit 1)")
            .arg(name);
    query.exec(checkName);

    while (query.next()) {
        qDebug() << query.value(0).toString();
        if (inputText == query.value(0).toString())
            flag = true;
        else
            flag = false;
    }
    return flag;
}

/**
  * @functionName Function Name : identityCheck(QString name, int inputText)
  * @brief Description: 判断用户身份是否正确
  * @date Date: 2018-7-4
  * @param Parameter: name 身份信息 inputText 要判断的数据
  * @return Return Code: true
  * @author Author: 张岩森
  *
  * */
bool identityCheck(QString name, int inputText)
{
    bool flag = false;

    QSqlQuery query;
    QString checkName = QString("select User_Is_Admin "
                                "from Sys_User "
                                "where User_Name = '%1' "
                                "limit 1")
            .arg(name);
    query.exec(checkName);

    while (query.next()) {
        if (inputText == query.value(0).toInt())
            flag = true;
        else
            flag = false;
    }
    return flag;
}

/*
 *
 *
 * */
/**
  * @functionName Function Name : recordSeller(QString input)
  * @brief Description: 记录卖家信息
  * 由于卖家身份和用户应该是一样的 一个卖家肯定有一个相应的用户
  * 此处只是模拟系统，不用检测是否卖家存在，因为一定存在
  * @date Date: 2018-7-4
  * @param Parameter: input 用户名
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
void recordSeller(QString input)
{
    QSqlQuery query;
    QString recordSqlSeller = QString("select * "
                                      "from Sys_Seller "
                                      "where Seller_Name = '%1' "
                                      "limit 1")
            .arg(input);
    query.exec(recordSqlSeller);

    while (query.next()) {
        User::id = query.value(0).toInt();
        User::name = query.value(1).toString();
        User::phone = query.value(2).toString();
        User::addr = query.value(3).toString();
    }
}

// 数据库操作函数
void sqlOperator(QString sql)
{
    QSqlQuery query;
    query.exec(sql);
}

/**
  * @functionName Function Name : checkExist(QString input, int number)
  * @brief Description: 根据查询条件来判断职工/卖家是否存在
  * @date Date: 2018-7-4
  * @param Parameter: input 输入条件 number 表的代号
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
bool checkExist(QString input, int number)
{
    bool flag = false;

    QSqlQuery query;
    QString checkSql;

    switch (number) {
    case STAFF_ID_NUMBER:
        checkSql = QString("select ifnull("
                           "(select Staff_Id "
                           "from Sys_Staff "
                           "where Staff_Id = '%1' "
                           "limit 1), "
                           "0)")
                .arg(input);
        break;
    case STAFF_DEPENTMENT_NUMBER:
        checkSql = QString("select ifnull("
                           "(select Staff_Deperment "
                           "from Sys_Staff "
                           "where Staff_Deperment = '%1' "
                           "limit 1), "
                           "0)")
                .arg(input);
        break;
    case STAFF_NAME_NUMBER:
        checkSql = QString("select ifnull("
                           "(select Staff_Name "
                           "from Sys_Staff "
                           "where Staff_Name = '%1' "
                           "limit 1), "
                           "0)")
                .arg(input);
        break;
    case SELLER_ID_NUMBER:
        checkSql = QString("select ifnull("
                           "(select Seller_Id "
                           "from Seller_View "
                           "where Seller_Id = '%1' "
                           "limit 1), "
                           "0)")
                .arg(input.toInt());
        break;
    case STORAGE_ID_NUMBER:
        checkSql = QString("select ifnull("
                           "(select storageID "
                           "from Seller_View "
                           "where storageID = '%1' "
                           "limit 1), "
                           "0)")
                .arg(input.toInt());
        break;
    default:
        break;
    }

    query.exec(checkSql);

    while (query.next()) {
        if (input == query.value(0).toString())
            flag = true;
        else
            flag = false;
    }
    return flag;
}

// 数据库操作函数
int Sale_Sql(const QString &sql)
{
    QSqlQuery query;
    query.exec(sql);
    // 这个地方初始化会 -1
    return query.lastError().number();
}

/**
  * @functionName Function Name : sellerViewNull()
  * @brief Description: 如果仓库信息表中卖家ID在卖家表中不存在
  * 管理员管理卖家信息时只显示卖家信息 其余信息为空
  * @date Date: 2018-7-4
  * @param Parameter: none
  * @return Return Code: none
  * @author Author: 张岩森
  *
  * */
bool sellerViewNull()
{
    bool flag = false;
    QString checkSql = QString("select count(*) "
                               "from Seller_View ");
    qDebug() << checkSql;

    QSqlQuery query;
    query.exec(checkSql);
    while (query.next()) {
        if (0 == query.value(0).toInt())
            flag = true;
        else
            flag = false;
    }
    return flag;
}

/**
  * @functionName Function Name : infoSeller(int id)
  * @brief Description: 管理员根据卖家ID查询卖家信息
  * @date Date: 2018-7-4
  * @param Parameter: id 卖家ID
  * @return Return Code: Seller 卖家信息
  * @author Author: 张岩森
  *
  * */
Seller infoSeller(int id)
{
    Seller seller;
    QSqlQuery query;
    QString sql = QString("select SellerName, SellerPhone, "
                          "select SellerAddr "
                          "from Sys_Seller "
                          "where Seller_Id = '%1'")
            .arg(id);
    query.exec(sql);
    while (query.next()) {
        seller.name = query.value(1).toString();
        seller.phone = query.value(2).toString();
        seller.addr = query.value(3).toString();
    }
    return seller;
}
