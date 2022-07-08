#include "pkgsearch.h"
#include "ui_pkgsearch.h"
#include "variables.h"
#include "QStandardItemModel"
#include "QStringList"
#include "QTableWidgetItem"
#include "QMessageBox"
#include "sstream"
#include "navigation.h"

static QStandardItemModel* model_search;
pkgSearch::pkgSearch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pkgSearch)
{
    //获得当前用户
    user* currentUser = Users.getuser(Users.getCurrentNum());
    //前端内容展示
    ui->setupUi(this);
    ui->name->setText(QString::fromLocal8Bit(currentUser->getName().data()));
    ui->type->addItem("收件人");
    ui->type->addItem("发件人");
    ui->type->addItem("寄出时间(格式:yyyy/mm/dd/hh/mm)");
    ui->type->addItem("收到时间(格式:yyyy/mm/dd/hh/mm)");
    ui->type->addItem("单号（序号）");
    model_search = new QStandardItemModel(ui->mainTable);
    ui->mainTable->setModel(model_search);
    model_search->setHorizontalHeaderItem(0, new QStandardItem("寄件人"));
    model_search->setHorizontalHeaderItem(1, new QStandardItem("收件人"));
    model_search->setHorizontalHeaderItem(2, new QStandardItem("寄件时间"));
    model_search->setHorizontalHeaderItem(3, new QStandardItem("收件地址"));
    model_search->setHorizontalHeaderItem(4, new QStandardItem("收件电话"));
    model_search->setHorizontalHeaderItem(5, new QStandardItem("状态"));
    ui->mainTable->setColumnWidth(0, 50);    //设置列的宽度
    ui->mainTable->setColumnWidth(1, 50);
    ui->mainTable->setColumnWidth(2, 150);
    ui->mainTable->setColumnWidth(3, 150);
    ui->mainTable->setColumnWidth(4, 150);
    ui->mainTable->setColumnWidth(5, 50);
}

pkgSearch::~pkgSearch()
{
    delete ui;
}

void pkgSearch::on_Get_clicked()
{
    //搜索快递
    //获得当前用户以鉴权
    user* currentUser = Users.getuser(Users.getCurrentNum());
    QStringList qlist;
    //搜索类别
    int searchID=ui->type->currentIndex();
    if(searchID==2||searchID==3){
        qlist = ui->keyWord->text().split('/');
    }
    int fail=-2;
    //根据搜索类别进行搜索并把结果存储到搜索结果数组中
    switch(searchID){
        case 0:
            fail=Packages.searchRecverBySender(currentUser->getName(),*currentUser,string(ui->keyWord->text().toLocal8Bit()));
            break;
        case 1:
            fail=Packages.searchSenderByRecver(currentUser->getName(),*currentUser,string(ui->keyWord->text().toLocal8Bit()));
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
        if(ui->keyWord->text().toInt()<Packages.size){
            Packages.checkSize=1;
            delete [] Packages.checkOutcome;
            Packages.checkOutcome=new int;
            Packages.checkOutcome[0]=ui->keyWord->text().toInt();

        }
        else fail=FAIL;
        break;

    }
    int i;
    //搜索结果展示
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
                     << new QStandardItem(QString::fromLocal8Bit(Packages.list[Packages.checkOutcome[i]].getStatus().data()));
             model_search->insertRow(i, list);
        }
        QMessageBox::warning(this,"查找完毕","查找完毕！ ");

    }


}
void pkgSearch::closeEvent(QCloseEvent *e){
    Navigation* navi = new Navigation;
    navi->showMaximized();
}
void pkgSearch::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}


