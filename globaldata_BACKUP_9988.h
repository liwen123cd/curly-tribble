#ifndef GLOBALDATA
#define GLOBALDATA
#include <QString>
#include <QDateTime>

// 全局变量 判断是否是管理员
// is_admin 为 1 表示是管理员
class Data
{
public:
    static int is_admin;
};

// 保存用户信息
class User
{
public:
    static int id;
    static QString name;
    static QString phone;
    static QString addr;
};

// 保存职工信息
class Staff
{
public:
    static QString id;
    static QString name;
    static QString deperment;
    static QString position;
    static QString status;
    static QString phone;
    static QString date;
    static QString image;
    static QString description;
};

// 保存卖家信息
class Seller
{
public:
    static int id;
    static QString name;
    static QString phone;
    static QString addr;
    static int number;
};

// 订单信息
struct Sale_Order_Detail {
    int Sale_Row;
    QString Sale_Order_ID;
    QString Sale_Buyer_Name;
    QString Sale_Buyer_Tel;
    QString Sale_Buyer_Address;
    int Sale_Item_ID;
    int Sale_Item_Num;
    float Sale_Item_Price;//单价
    QString Sale_State;
    bool Sale_IS_Dirty;//是否可改

};

// 订单修改记录的信息
struct Sale_State_Detail {
    QString Sale_Order_ID;
    QString Sale_Order_State;
    QDateTime Sale_Date;
};

// 商品信息
struct Product_Detail {
<<<<<<< HEAD
	//商品信息
	QString Product_Name;
	float Product_Price;
	QString Product_Provider;
    QString Path;
=======
    QString Product_Name;
    float Product_Price;
    QString Product_Provider;
>>>>>>> 1fc06df99f1868a8946743a7b494dc20baa5bf0b

};

#endif // GLOBALDATA
