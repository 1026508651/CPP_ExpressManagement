#include "admincheck.h"
#include "ui_admincheck.h"
#include "variables.h"
#include "login.h"
#include "adminpkgsearch.h"
#include "adminusercheck.h"
int call_admin=0;
static CSocket* socket=CSocket::instance();
AdminCheck::AdminCheck(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminCheck)
{
    ui->setupUi(this);

    user currentUser = Users.getuser(Users.getCurrentNum());
    ui->welcome_name->setText(QString::fromLocal8Bit(currentUser.getName(currentUser).data()));

}

AdminCheck::~AdminCheck()
{
    delete ui;
}
void AdminCheck::closeEvent(QCloseEvent *e){

    if(call_admin==0){
        login *log=new login;
        int currentNum=Users.getCurrentNum();
        Users.userLogout(currentNum);

        log->showMaximized();
    }
    else call_admin=0;
}
void AdminCheck::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}
void AdminCheck::on_search_clicked()
{
    call_admin=1;
    adminPkgSearch * adminpkg=new adminPkgSearch;
    adminpkg->showMaximized();
    this->close();
}


void AdminCheck::on_safety_clicked()
{
    call_admin=1;
    adminUserCheck* usercheck=new adminUserCheck;
    usercheck->showMaximized();
    this->close();
}
