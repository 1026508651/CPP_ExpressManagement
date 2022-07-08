#include "admincheck.h"
#include "ui_admincheck.h"
#include "variables.h"
#include "login.h"
#include "adminpkgsearch.h"
#include "adminusercheck.h"
static int call_admin=0;//用于区分窗口是否是用户主动关闭的，如果是则返回上一层，否则不主动打开新窗口（因为关闭之前已经打开新窗口）

AdminCheck::AdminCheck(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminCheck)
{
    ui->setupUi(this);
    //当前用户查看
    user* currentUser = Users.getuser(Users.getCurrentNum());
    ui->welcome_name->setText(QString::fromLocal8Bit(currentUser->getName(*currentUser).data()));

}

AdminCheck::~AdminCheck()
{
    delete ui;
}
void AdminCheck::closeEvent(QCloseEvent *e){

    if(call_admin==0){
        login *log=new login;
        Users.getuser(Users.getCurrentNum())->state=USER_UNLOGIN;
        Users.setCurrentNum(FAIL);//退出用户登陆状态自动为-1
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
    adminPkgSearch * adminpkg=new adminPkgSearch;//查快递
    adminpkg->showMaximized();
    this->close();
}


void AdminCheck::on_safety_clicked()
{
    call_admin=1;
    adminUserCheck* usercheck=new adminUserCheck;//查用户
    usercheck->showMaximized();
    this->close();
}
