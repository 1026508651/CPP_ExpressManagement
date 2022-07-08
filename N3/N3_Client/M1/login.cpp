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
static int call=0;
extern int isFirst;
static CSocket* socket=CSocket::instance();
login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    if(isFirst==0){
        socket->onConnect(QUrl(QStringLiteral("ws://localhost:12345")));
        isFirst=1;
    }

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
    call=1;
    this->close();
}
void login::closeEvent(QCloseEvent *e){

    if(call==0){
        socket->closed();
    }
    else{
        call=0;
    }
}

void login::on_login_2_clicked()
{

    user tempLogin=user(0,string(ui->username->text().toLocal8Bit()),"",string(ui->passwordInput->text().toLocal8Bit()),"");
    int status;
    status=Users.login(tempLogin);

    cout<<"shit!"<<status<<endl;
    if(status==FAIL){
        QMessageBox::warning(this,"提示","用户名或密码，请检查后重试! ");
    }
    else if(status==Users.getSize()){
        QMessageBox::warning(this,"提示","用户名或密码，请检查后重试! ");
    }
    else{
        call=1;
        cout<<status<<endl;
        cout<<status<<endl;
        Users.setCurrentNum(status);
        user currentUser=Users.getuser(status);
        if(currentUser.character==ADMIN_USER){//管理员用户
            AdminCheck* admincheck= new AdminCheck;
            admincheck->showMaximized();
            this->close();

        }
        else if(currentUser.character==NORMAL_USER){//普通用户

            Navigation* navigate=new Navigation;

            navigate->showMaximized();

            this->close();

        }
        else if(Users.getuser(status).character==PACKAGE_USER){
            pkgManNavi* navi=new pkgManNavi;
            navi->showMaximized();
            this->close();
        }

    }

}


