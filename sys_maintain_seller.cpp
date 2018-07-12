#include "sys_maintain_seller.h"
#include "ui_sys_maintain_seller.h"

Sys_Maintain_Seller::Sys_Maintain_Seller(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sys_Maintain_Seller)
{
    ui->setupUi(this);
}

Sys_Maintain_Seller::~Sys_Maintain_Seller()
{
    delete ui;
}
