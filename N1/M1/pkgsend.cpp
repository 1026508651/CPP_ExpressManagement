#include "pkgsend.h"
#include "ui_pkgsend.h"
#include "QInputDialog"
#include "string"
#include "QMessageBox"
#include "navigation.h"
#include "variables.h"
using namespace std;

pkgSend::pkgSend(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pkgSend)
{
    //获取当前用户
    user* currentUser = Users.getuser(Users.getCurrentNum());
    ui->setupUi(this);
    int i;
    //展示用户地址
    for (i=0;i<currentUser->addressSize;i++) {
        ui->recvAdressCombo->addItem(QString::fromLocal8Bit(currentUser->address[i].data()));
        ui->sendAdressCombo->addItem(QString::fromLocal8Bit(currentUser->address[i].data()));
    }
}
void pkgSend::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}
pkgSend::~pkgSend()
{
    delete ui;
}

void pkgSend::on_adressAdd_clicked()
{
    //用户添加地址
    bool ok;
    // 获取字符串
    string str= string(QInputDialog::getText(this, "添加地址" , "请输入待添加的地址：" , QLineEdit::Normal,tr("" ), &ok).toLocal8Bit());
    if(ok){
        if(Users.getuser(Users.getCurrentNum())->addAdress(str)!=FAIL){
            QMessageBox::warning(this,"添加成功","地址添加成功，请重新填写！ ");
            this->close();
        }
        else{
            QMessageBox::warning(this,"添加失败","地址添加失败，现有地址满！ ");
        }
    }
}
void pkgSend::closeEvent(QCloseEvent *e){
    Navigation* navi = new Navigation;
    navi->showMaximized();
}
void pkgSend::on_Send_clicked()
{
    //发快递 从各个要素获取数据
    user* currentUser = Users.getuser(Users.getCurrentNum());
    string recvName=string(ui->recvName->text().toLocal8Bit());
    string recvPhone=string(ui->recvPhone->text().toLocal8Bit());
    int recvad=ui->recvAdressCombo->currentIndex();
    int sendad=ui->sendAdressCombo->currentIndex();
    string pkgDes=string(ui->pkgDes->text().toLocal8Bit());
    if(Users.searchByName(recvName)!=0){
        //调用快递发送函数
        if(currentUser->sendPkg(Packages.size,sendad,recvName,recvad,recvPhone,pkgDes)!=FAIL){
            QMessageBox::warning(this,"寄件成功","寄件成功！ ");
            this->close();
        }
        else{
            QMessageBox::warning(this,"寄件失败","余额不足，寄件失败！ ");
        }
    }
    else{
        QMessageBox::warning(this,"寄件失败","寄件失败！请检查收件人是否合法（在用户数据库中）！ ");
    }
}
