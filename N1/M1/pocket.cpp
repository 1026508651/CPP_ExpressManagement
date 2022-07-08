#include "pocket.h"
#include "ui_pocket.h"
#include "navigation.h"
#include "QInputDialog"
#include "QMessagebox"
#include <sstream>
pocket::pocket(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pocket)
{
    ui->setupUi(this);
    user* currentUser = Users.getuser(Users.getCurrentNum());
    stringstream ss;
    string s;
    ss<<currentUser->getMoney();
    ss>>s;
    ui->money_left->setText(QString::fromLocal8Bit(s.data()));
    ui->name->setText(QString::fromLocal8Bit(currentUser->getName().data()));

}
void pocket::closeEvent(QCloseEvent *e){
    Navigation* navi = new Navigation;
    navi->showMaximized();
}
void pocket::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}


pocket::~pocket()
{
    delete ui;
}

void pocket::on_addMoney_clicked()
{
    //账户充值函数
    stringstream ss;
    string s;
    user* currentUser = Users.getuser(Users.getCurrentNum());
    bool ok;
    int money_Num= QInputDialog::getText(this, "充值金额" , "请输入充值金额：" , QLineEdit::Normal,tr("" ), &ok).toInt();
    if(ok){
        string passwd= string(QInputDialog::getText(this, "输入密码" , "请输入登陆密码：" , QLineEdit::EchoMode::Password,tr("" ), &ok).toLocal8Bit());
        if(currentUser->checkPasswd(passwd)==0&&ok&&money_Num>0){
            currentUser->addMoney(money_Num);


            ss<<"充值"<<money_Num<<"元成功！";
            ss>>s;

            QMessageBox::warning(this,"充值成功",QString(s.data()));
            stringstream s2;
            s2<<currentUser->getMoney();
            string s1;
            s2>>s1;
            ui->money_left->setText(QString::fromLocal8Bit(s1.data()));
        }
        else{
            QMessageBox::warning(this,"充值失败","输入错误！充值失败！");
        }

    }
}
