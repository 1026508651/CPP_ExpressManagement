#include "login.h"
#include "ui_login.h"
#include "variables.h"
#include <iostream>
#include <QMessageBox>
#include "registerwindow.h"
#include "navigation.h"
#include "admincheck.h"
#include "pkgmannavi.h"
using namespace std;
login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->passwordInput->setEchoMode(QLineEdit::EchoMode::Password);
    QFrame *mainFrame = this->findChild<QFrame *>("mainFrame");

}
void login::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralWidget->width(), height = ui->centralWidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}
login::~login()
{
    delete ui;
}

void login::on_register_2_clicked()
{
    RegisterWindow* registerwindow=new RegisterWindow;
    registerwindow->showMaximized();
    this->close();
}
void login::closeEvent(QCloseEvent *e){
    //关闭程序 保存数据
    Users.saveData(userLocal);
    Packages.saveData(packageLocal);
    cout<<Users.getSize()<<endl;

}

void login::on_login_2_clicked()
{
    //登录 首先初始化暂存用户 输入用户名密码
    user tempLogin=user(0,string(ui->username->text().toLocal8Bit()),"",string(ui->passwordInput->text().toLocal8Bit()),"");
    int status;
    status=Users.login(tempLogin);
    //根据登陆函数返回结果决定下一步动作
    cout<<"shit!"<<status<<endl;
    if(status==FAIL){
        QMessageBox::warning(this,"提示","用户名或密码，请检查后重试! ");
    }
    else if(status==Users.getSize()){
        QMessageBox::warning(this,"提示","用户名或密码，请检查后重试! ");
    }
    //FAIl或者超限均提示登录失败
    else{
        cout<<status<<endl;
        cout<<status<<endl;
        Users.setCurrentNum(status);//设置当前用户
        if(Users.getuser(status)->character==ADMIN_USER){//管理员用户
            AdminCheck* admincheck= new AdminCheck;
            admincheck->showMaximized();
            this->close();

        }
        else if(Users.getuser(status)->character==NORMAL_USER){//普通用户

            Navigation* navigate=new Navigation;

            navigate->showMaximized();

            this->close();

        }
        else if(Users.getuser(status)->character==PACKAGE_USER){//快递员
            pkgManNavi* navi=new pkgManNavi;
            navi->showMaximized();
            this->close();
        }

    }

}
