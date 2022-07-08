#include "navigation.h"
#include "ui_navigation.h"
#include "pkgsend.h"
#include "login.h"
#include "pkgrev.h"
#include "pkgsearch.h"
#include "pocket.h"
#include "safety.h"
static int call=0;
static CSocket* socket=CSocket::instance();//全局socket
Navigation::Navigation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Navigation)
{
    ui->setupUi(this);

    cout<<"status:"<<Users.getCurrentNum()<<endl;
    user currentUsr=Users.getuser(Users.getCurrentNum());
    ui->welcome->setText(QString::fromLocal8Bit(currentUsr.getName(currentUsr).data()));
}

Navigation::~Navigation()
{
    delete ui;
}

void Navigation::on_search_clicked()
{
    pkgSearch* search=new pkgSearch;
    search->showMaximized();
    call=1;
    this->close();
}
void Navigation::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    int width = ui->centralwidget->width(), height = ui->centralwidget->height();
       ui->mainFrame->move(width/2-400, height/3-150);
    // Your code here
 }
void Navigation::on_recv_clicked()
{
    pkgRev *recv=new pkgRev;
    recv->showMaximized();
    call=1;
    this->close();
}

void Navigation::on_send_clicked()
{
    pkgSend *sending=new pkgSend;
    sending->showMaximized();
    call=1;
    this->close();
}

void Navigation::on_safety_clicked()
{
    Safety* safe=new Safety;
    safe->showMaximized();
    call=1;
    this->close();
}

void Navigation::on_account_clicked()
{
    pocket* pock=new pocket;
    pock->showMaximized();
    call=1;
    this->close();
}
void Navigation::closeEvent(QCloseEvent *e){
    if(call==0){
        login *log=new login;

        Users.userLogout(Users.getCurrentNum());
        log->showMaximized();
    }
    else call=0;

}
