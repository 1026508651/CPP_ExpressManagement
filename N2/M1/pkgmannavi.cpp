#include "pkgmannavi.h"
#include "ui_pkgmannavi.h"
#include "login.h"
#include "pkgmanrecv.h"
#include "pkgmancheck.h"
#include "userssafty.h"
extern int currentPkgMan;
int currentPkgMan=-1;
static int call=0;
pkgManNavi::pkgManNavi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pkgManNavi)
{
    ui->setupUi(this);
    user* currentUser = Users.getuser(Users.getCurrentNum());
    ui->welcome_name->setText(QString::fromLocal8Bit(currentUser->getName(*currentUser).data()));

}

pkgManNavi::~pkgManNavi()
{
    delete ui;
}

void pkgManNavi::on_search_clicked()
{
    pkgManRecv *recv=new pkgManRecv;
    recv->showMaximized();
    call=1;
    this->close();
}
void pkgManNavi::closeEvent(QCloseEvent *e){
    if(call==0){
        login *log=new login;
        Users.getuser(Users.getCurrentNum())->state=USER_UNLOGIN;
        Users.setCurrentNum(-1);//退出用户登陆状态自动为-1
        log->showMaximized();
    }
    else call=0;
}
void pkgManNavi::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}


void pkgManNavi::on_check_clicked()
{
    pkgManCheck* check=new pkgManCheck;
    check->showMaximized();
    call=1;
    this->close();
}

void pkgManNavi::on_user_2_clicked()
{
    usersSafty *recv=new usersSafty;
    recv->showMaximized();
    call=1;
    this->close();
}
