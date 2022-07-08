#include "pkgmancheck.h"
#include "ui_pkgmancheck.h"
#include "adminpkgsearch.h"
#include "ui_adminpkgsearch.h"
#include "QStandardItemModel"
#include "variables.h"
#include "QMessageBox"
#include "QTableWidgetItem"
#include "pkgmannavi.h"
#include "adminusercheck.h"
#include "userssafty.h"
extern int currentPkgMan;

static QStandardItemModel* model_search;
pkgManCheck::pkgManCheck(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pkgManCheck)
{

    user* currentUser;
    ui->setupUi(this);
    if(currentPkgMan==FAIL){
        currentUser=Users.getuser(Users.getCurrentNum());
        ui->name_2->setText(QString::fromLocal8Bit(currentUser->getName(*currentUser).data()));
    }
    else{
        currentUser=Users.getuser(currentPkgMan);
        ui->name_2->setText(QString("管理员 ")+QString::fromLocal8Bit(Users.getuser(Users.getCurrentNum())->getName(*Users.getuser(Users.getCurrentNum())).data()));
        ui->attention->setText(QString("这里是快递员")+QString::fromLocal8Bit(currentUser->getName(*currentUser).data())+QString("的快递记录"));
    }



    ui->type_2->addItem("收件人");
    ui->type_2->addItem("发件人");
    ui->type_2->addItem("寄出时间(格式:yyyy/mm/dd/hh/mm)");
    ui->type_2->addItem("收到时间(格式:yyyy/mm/dd/hh/mm)");
    ui->type_2->addItem("单号（序号）");
    model_search = new QStandardItemModel(ui->mainTable_2);
    ui->mainTable_2->setModel(model_search);
    model_search->setHorizontalHeaderItem(0, new QStandardItem("寄件人"));
    model_search->setHorizontalHeaderItem(1, new QStandardItem("收件人"));
    model_search->setHorizontalHeaderItem(2, new QStandardItem("寄件时间"));
    model_search->setHorizontalHeaderItem(3, new QStandardItem("收件地址"));
    model_search->setHorizontalHeaderItem(4, new QStandardItem("收件电话"));
    model_search->setHorizontalHeaderItem(5, new QStandardItem("状态"));
    model_search->setHorizontalHeaderItem(6, new QStandardItem("快递员"));
    ui->mainTable_2->setColumnWidth(0, 50);    //设置列的宽度
    ui->mainTable_2->setColumnWidth(1, 50);
    ui->mainTable_2->setColumnWidth(2, 150);
    ui->mainTable_2->setColumnWidth(3, 150);
    ui->mainTable_2->setColumnWidth(4, 150);
    ui->mainTable_2->setColumnWidth(5, 50);
    ui->mainTable_2->setColumnWidth(6, 100);
}

pkgManCheck::~pkgManCheck()
{
    delete ui;
}



void pkgManCheck::closeEvent(QCloseEvent *e){
    if(currentPkgMan==FAIL){
        pkgManNavi* navi = new pkgManNavi;
        navi->showMaximized();
    }
    else{
        adminUserCheck* check=new adminUserCheck;
        check->showMaximized();
    }
}
void pkgManCheck::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}

void pkgManCheck::on_Get_2_clicked()
{
    user* currentUser;
    if(currentPkgMan==FAIL){
        currentUser=Users.getuser(Users.getCurrentNum());

    }
    else{
        currentUser=Users.getuser(currentPkgMan);
    }

    QStringList qlist;
    int fail=-2;
    int i;
    int searchID=ui->type_2->currentIndex();
    if(searchID==2||searchID==3){
        qlist = ui->keyWord_2->text().split('/');
    }

    if(ui->keyWord_2->text().isEmpty()){
        Packages.checkALL(*currentUser);
    }
    else{
        switch(searchID){
            case 0:
                fail=Packages.searchRecverBySender(currentUser->getName(*currentUser),*currentUser,string(ui->keyWord_2->text().toLocal8Bit()));
                break;
            case 1:
                fail=Packages.searchSenderByRecver(currentUser->getName(*currentUser),*currentUser,string(ui->keyWord_2->text().toLocal8Bit()));
                break;
            case 2:
                if(qlist.size()==5){
                    unsigned int time=qlist[0].toUInt()*100000000+qlist[1].toUInt()*1000000+qlist[2].toUInt()*10000+qlist[3].toUInt()*100+qlist[4].toUInt();
                    fail=Packages.searchByTime(time,time,1,*currentUser);
                }
                break;
            case 3:
                if(qlist.size()==5){
                    unsigned int time=qlist[0].toUInt()*100000000+qlist[1].toUInt()*1000000+qlist[2].toUInt()*10000+qlist[3].toUInt()*100+qlist[4].toUInt();
                    fail=Packages.searchByTime(time,time,2,*currentUser);
                }
                break;
            case 4:
            if(ui->keyWord_2->text().toInt()<Packages.size){
                Packages.checkSize=1;
                delete [] Packages.checkOutcome;
                Packages.checkOutcome=new int;
                Packages.checkOutcome[0]=ui->keyWord_2->text().toInt();

            }
            else fail=FAIL;
            break;

        }
    }



    model_search->removeRows(0,model_search->rowCount());
    if(fail==FAIL||Packages.checkSize==0){
        QMessageBox::warning(this,"查找失败","查找失败！无结果！ ");
    }
    else{
        for(i=0;i<Packages.checkSize;i++){
             QList<QStandardItem*> list;
             QTableWidgetItem* p_check = new QTableWidgetItem();
             p_check->setCheckState(Qt::Unchecked);
             list << new QStandardItem(QString::fromLocal8Bit(Packages.list[Packages.checkOutcome[i]].getSender().data()))
                     << new QStandardItem(QString::fromLocal8Bit(Packages.list[Packages.checkOutcome[i]].getRecver().data()))
                     << new QStandardItem(QString::fromLocal8Bit(timeconvert(Packages.list[Packages.checkOutcome[i]].sendTime).data()))
                     << new QStandardItem(QString::fromLocal8Bit(Packages.list[Packages.checkOutcome[i]].getRecvAdress().data()))
                     << new QStandardItem(QString::fromLocal8Bit(Packages.list[Packages.checkOutcome[i]].getRecvPhone().data()))
                     << new QStandardItem(QString::fromLocal8Bit(Packages.list[Packages.checkOutcome[i]].getStatus().data()))
                     << new QStandardItem(QString::fromLocal8Bit(Users.getuser(Packages.list[Packages.checkOutcome[i]].getPkgMan())->getName(*Users.getuser(Packages.list[Packages.checkOutcome[i]].getPkgMan())).data()));
             model_search->insertRow(i, list);
        }
        QMessageBox::warning(this,"查找完毕","查找完毕！ ");

    }
}
