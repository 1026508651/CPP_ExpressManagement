#include "pkgmanregister.h"
#include "ui_pkgmanregister.h"
#include <QMessageBox>
#include <QTextCodec>
#include<iostream>
#include "variables.h"
#include "login.h"
#include "adminusercheck.h"
static CSocket* socket=CSocket::instance();
pkgManRegister::pkgManRegister(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pkgManRegister)
{
    ui->setupUi(this);

    ui->passwordInput->setEchoMode(QLineEdit::EchoMode::Password);
    ui->passwordConfirm->setEchoMode(QLineEdit::EchoMode::Password);
}

pkgManRegister::~pkgManRegister()
{
    delete ui;
}


void pkgManRegister::on_Register_clicked()
{


//    QString str;
//    str=ui->name->text(); //取得LineEdit1返回的文字
//    QByteArray cdata = str.toLocal8Bit();
//    cout<<string(cdata)<<endl;
    if(string(ui->name->text().toLocal8Bit()).size()==0){
        QMessageBox::warning(this,"提示","姓名输入不合法，请检查后重试! ");
        return;
    }
    if(string(ui->username->text().toLocal8Bit()).size()==0){
        QMessageBox::warning(this,"提示","用户名输入不合法，请检查后重试! ");
        return;
    }
    if(string(ui->passwordInput->text().toLocal8Bit()).size()<8){
        QMessageBox::warning(this,"提示","密码输入不合法，要求在8位及以上，请检查后重试! ");
        return;
    }
    if(string(ui->passwordConfirm->text().toLocal8Bit()).size()<8){
        QMessageBox::warning(this,"提示","密码确认输入不合法，要求在8位及以上，请检查后重试! ");
        return;
    }
    if(string(ui->phoneNumber->text().toLocal8Bit()).size()==0){
        QMessageBox::warning(this,"提示","电话输入不合法，请检查后重试! ");
        return;
    }
    if(checkPassword()!=0){
        QMessageBox::warning(this,"提示","两次密码输入不一致，请重新输入! ");
        return;
    }
    if(FAIL!=Users.registerUser(string(ui->username->text().toLocal8Bit()),string(ui->name->text().toLocal8Bit()),string(ui->passwordConfirm->text().toLocal8Bit()),string(ui->phoneNumber->text().toLocal8Bit()),PACKAGE_USER)){
        QMessageBox::warning(this,"提示","注册成功!");
        cout<<Users.getSize()<<endl;
        cout<<string(ui->name->text().toLocal8Bit())<<endl;
        this->close();
    }
    else{

        QMessageBox::warning(this,"提示","注册失败！用户名重复，请更换后重试！ ");
    }
}
int pkgManRegister::checkPassword(){
    //cout<<"Checking!"<<endl;
    int a=string(ui->passwordInput->text().toLocal8Bit()).compare(string(ui->passwordConfirm->text().toLocal8Bit()));
    cout<<a<<endl;
    return a;
}
void pkgManRegister::closeEvent(QCloseEvent *e){
    cout<<"PkgMan Register close!"<<endl;
    adminUserCheck* w=new adminUserCheck;
    w->showMaximized();
}
void pkgManRegister::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
   ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}
