#include "adminpkgsearch.h"
#include "ui_adminpkgsearch.h"
#include "QStandardItemModel"
#include "variables.h"
#include "QMessageBox"
#include "QTableWidgetItem"
#include "admincheck.h"
static QStandardItemModel* model_adminsearch;
static CSocket* socket=CSocket::instance();
adminPkgSearch::adminPkgSearch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminPkgSearch)
{
    int userNum=Users.getCurrentNum();

    user currentUser = Users.getuser(userNum);

    ui->setupUi(this);
    ui->name->setText(QString::fromLocal8Bit(currentUser.getName(currentUser).data()));
    ui->type->addItem("收件人");
    ui->type->addItem("发件人");
    ui->type->addItem("寄出时间(格式:yyyy/mm/dd/hh/mm)");
    ui->type->addItem("收到时间(格式:yyyy/mm/dd/hh/mm)");
    ui->type->addItem("单号（序号）");
    model_adminsearch = new QStandardItemModel(ui->mainTable);
    ui->mainTable->setModel(model_adminsearch);
    model_adminsearch->setHorizontalHeaderItem(0, new QStandardItem("寄件人"));
    model_adminsearch->setHorizontalHeaderItem(1, new QStandardItem("收件人"));
    model_adminsearch->setHorizontalHeaderItem(2, new QStandardItem("寄件时间"));
    model_adminsearch->setHorizontalHeaderItem(3, new QStandardItem("收件地址"));
    model_adminsearch->setHorizontalHeaderItem(4, new QStandardItem("收件电话"));
    model_adminsearch->setHorizontalHeaderItem(5, new QStandardItem("状态"));
     model_adminsearch->setHorizontalHeaderItem(6, new QStandardItem("快递员"));
    ui->mainTable->setColumnWidth(0, 50);    //设置列的宽度
    ui->mainTable->setColumnWidth(1, 50);
    ui->mainTable->setColumnWidth(2, 150);
    ui->mainTable->setColumnWidth(3, 150);
    ui->mainTable->setColumnWidth(4, 150);
    ui->mainTable->setColumnWidth(5, 50);
    ui->mainTable->setColumnWidth(6, 100);
}

adminPkgSearch::~adminPkgSearch()
{
    delete ui;
}

void adminPkgSearch::on_Get_clicked()
{
    int userNum=Users.getCurrentNum();
    user currentUser = Users.getuser(userNum);
    QStringList qlist;
    int fail=-2;
    int i;
    QString keyWord;
    int searchID=ui->type->currentIndex();
    if(searchID==2||searchID==3){
        qlist = ui->keyWord->text().split('/');
    }

    if(ui->keyWord->text().isEmpty()){
//        Packages.checkSize=Packages.size;
//        delete [] Packages.checkOutcome;
//        Packages.checkOutcome=new int[Packages.checkSize];
//        for(i=0;i<Packages.checkSize;i++){
//            Packages.checkOutcome[i]=i;
//        }
        keyWord="*";
    }
    else{
        switch(searchID){
            case 0:
                //fail=Packages.searchRecverBySender(currentUser->getName(*currentUser),*currentUser,string(ui->keyWord->text().toLocal8Bit()));
                keyWord=ui->keyWord->text();
                break;
            case 1:
                //fail=Packages.searchSenderByRecver(currentUser->getName(*currentUser),*currentUser,string(ui->keyWord->text().toLocal8Bit()));
                keyWord=ui->keyWord->text();
                break;
            case 2:
                if(qlist.size()==5){
                    unsigned int time=qlist[0].toUInt()*100000000+qlist[1].toUInt()*1000000+qlist[2].toUInt()*10000+qlist[3].toUInt()*100+qlist[4].toUInt();
                    keyWord=QString::number(time)+"|1";
                    //fail=Packages.searchByTime(time,time,1,*currentUser);
                }
                break;
            case 3:
                if(qlist.size()==5){
                    unsigned int time=qlist[0].toUInt()*100000000+qlist[1].toUInt()*1000000+qlist[2].toUInt()*10000+qlist[3].toUInt()*100+qlist[4].toUInt();
                    keyWord=QString::number(time)+"|2";
                    //fail=Packages.searchByTime(time,time,2,*currentUser);
                }
                break;
            case 4:
            if(ui->keyWord->text().toInt()<Packages.getSize()){
//                Packages.checkSize=1;
//                delete [] Packages.checkOutcome;
//                Packages.checkOutcome=new int;
//                Packages.checkOutcome[0]=ui->keyWord->text().toInt();
                keyWord=ui->keyWord->text();

            }
            else fail=FAIL;
            break;

        }
    }
    fail=Packages.search(searchID,keyWord,userNum);
    int saveCheckSize=Packages.getCheckSize();

    model_adminsearch->removeRows(0,model_adminsearch->rowCount());
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
                     user pkgMan=   Users.getuser(pkgSave.getPkgMan());
                     list<< new QStandardItem(QString::fromLocal8Bit(pkgMan.getName(pkgMan).data()));
             model_adminsearch->insertRow(i, list);
        }
        QMessageBox::warning(this,"查找完毕","查找完毕！ ");

    }
}
void adminPkgSearch::closeEvent(QCloseEvent *e){
    AdminCheck* admin_navi = new AdminCheck;
    admin_navi->showMaximized();
}
void adminPkgSearch::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}
