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
extern int currentPkgMan;
static CSocket* socket=CSocket::instance();
static QStandardItemModel* model_search;
pkgManCheck::pkgManCheck(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pkgManCheck)
{

    user currentUser;
    int currentNum;
    ui->setupUi(this);
    if(currentPkgMan==FAIL){
        currentNum=Users.getCurrentNum();
        currentUser=Users.getuser(currentNum);
        ui->name_2->setText(QString::fromLocal8Bit(currentUser.getName(currentUser).data()));
    }
    else{
        currentUser=Users.getuser(currentPkgMan);
        currentNum=currentPkgMan;

        ui->name_2->setText(QString("管理员 ")+QString::fromLocal8Bit(currentUser.getName(currentUser).data()));
        ui->attention->setText(QString("这里是快递员")+QString::fromLocal8Bit(currentUser.getName(currentUser).data())+QString("的快递记录"));
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
    user currentUser;
    int currentNum;
    QString keyWord;
    if(currentPkgMan==FAIL){
        currentNum=Users.getCurrentNum();
    }
    else{
        currentNum=currentPkgMan;

    }
    currentUser=Users.getuser(currentNum);
    QStringList qlist;
    int fail=-2;
    int i;

    int searchID=ui->type_2->currentIndex();
    if(ui->keyWord_2->text().isEmpty()){
        keyWord='*';
    }
    else{
        if(searchID==2||searchID==3){
            qlist = ui->keyWord_2->text().split('/');
        }
        else{
            switch(searchID){
                case 0:
                    keyWord=ui->keyWord_2->text();
                break;
                case 1:
                    keyWord=ui->keyWord_2->text();
                break;
                case 2:
                    if(qlist.size()==5){
                        unsigned int time=qlist[0].toUInt()*100000000+qlist[1].toUInt()*1000000+qlist[2].toUInt()*10000+qlist[3].toUInt()*100+qlist[4].toUInt();
                        keyWord=QString::number(time)+"|1";
                    }
                    break;
                case 3:
                    if(qlist.size()==5){
                        unsigned int time=qlist[0].toUInt()*100000000+qlist[1].toUInt()*1000000+qlist[2].toUInt()*10000+qlist[3].toUInt()*100+qlist[4].toUInt();
                        keyWord=QString::number(time)+"|2";
                    }
                    break;
                case 4:
                if(ui->keyWord_2->text().toInt()<Packages.getSize()){
//                    Packages.checkSize=1;
//                    delete [] Packages.checkOutcome;
//                    Packages.checkOutcome=new int;
//                    Packages.checkOutcome[0]=ui->keyWord_2->text().toInt();
                    keyWord=ui->keyWord_2->text();
                }
                else fail=FAIL;
                break;

            }
    }

    }
    Packages.search(searchID,keyWord,currentNum);



    model_search->removeRows(0,model_search->rowCount());
     int saveCheckSize=Packages.getCheckSize();
    if(fail==FAIL||saveCheckSize==0){
        QMessageBox::warning(this,"查找失败","查找失败！无结果！ ");
    }
    else{
        int *saveOutcome=Packages.getCheckOutcom();
        for(i=0;i<saveCheckSize;i++){
             QList<QStandardItem*> list;
             QTableWidgetItem* p_check = new QTableWidgetItem();
             p_check->setCheckState(Qt::Unchecked);
             package pkgSave=Packages.getPkg(saveOutcome[i]);
             list << new QStandardItem(QString::fromLocal8Bit(pkgSave.getSender().data()))
                     << new QStandardItem(QString::fromLocal8Bit(pkgSave.getRecver().data()))
                     << new QStandardItem(QString::fromLocal8Bit(timeconvert(pkgSave.sendTime).data()))
                     << new QStandardItem(QString::fromLocal8Bit(pkgSave.getRecvAdress().data()))
                     << new QStandardItem(QString::fromLocal8Bit(pkgSave.getRecvPhone().data()))
                     << new QStandardItem(QString::fromLocal8Bit(pkgSave.getStatus().data()));
             user savePkgMan=Users.getuser(pkgSave.getPkgMan());
                     list<< new QStandardItem(QString::fromLocal8Bit(savePkgMan.getName(savePkgMan).data()));

             model_search->insertRow(i, list);
        }
        QMessageBox::warning(this,"查找完毕","查找完毕！ ");

    }
}
