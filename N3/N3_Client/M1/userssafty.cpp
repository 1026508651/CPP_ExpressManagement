#include "userssafty.h"
#include "ui_userssafty.h"
#include "variables.h"
#include "QMessageBox"
#include "QInputDialog"
#include "pkgmannavi.h"
#include "login.h"
usersSafty::usersSafty(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::usersSafty)
{
    ui->setupUi(this);
    user currentUser = Users.getuser(Users.getCurrentNum());
    ui->name_2->setText(QString::fromLocal8Bit(currentUser.getName(currentUser).data()));
    ui->username_2->setText(QString::fromLocal8Bit(currentUser.getUsername().data()));
    ui->phoneNum_2->setText(QString::fromLocal8Bit(currentUser.phoneNumber.data()));
    ui->phoneNum_3->setText(QString::number(currentUser.getMoney()));
}

usersSafty::~usersSafty()
{
    delete ui;
}
void usersSafty::closeEvent(QCloseEvent *e){
    if(Users.getCurrentNum()!=FAIL){
        pkgManNavi* navi = new pkgManNavi;
        navi->showMaximized();
    }
    else{
        login* log=new login;
        log->showMaximized();

    }
}
void usersSafty::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
   ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}
void usersSafty::on_Changepasswd_2_clicked()
{
    bool ok,ok2;
    user currentUser = Users.getuser(Users.getCurrentNum());
    string passwd= string(QInputDialog::getText(this, "输入密码" , "请输入登陆密码：" , QLineEdit::EchoMode::Password,tr("" ), &ok).toLocal8Bit());
    if(ok&&currentUser.checkPasswd(passwd)==0){
        string newpasswd= string(QInputDialog::getText(this, "输入新密码" , "请输入新密码：" , QLineEdit::EchoMode::Password,tr("" ), &ok2).toLocal8Bit());
        if(newpasswd.size()>=8){
            string repeatpasswd= string(QInputDialog::getText(this, "确认新密码" , "请再次输入新密码：" , QLineEdit::EchoMode::Password,tr("" ), &ok2).toLocal8Bit());
            if(newpasswd.compare(repeatpasswd)==0){

                currentUser.changePasswd(newpasswd);
                QMessageBox::warning(this,"修改成功","修改成功！请重新登录！ ");
                Users.userLogout(Users.getCurrentNum());
                this->close();
            }
            else{
                QMessageBox::warning(this,"修改失败","修改失败！两次输入新密码不同！ ");
            }
        }
        else{
          QMessageBox::warning(this,"修改失败","修改失败！新密码长度必须大于8位！ ");
        }
    }
    else{
        QMessageBox::warning(this,"修改失败","修改失败！原有密码输入错误！ ");
    }
}


