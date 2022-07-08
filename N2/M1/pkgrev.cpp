#include "pkgrev.h"
#include "ui_pkgrev.h"
#include "navigation.h"
#include "variables.h"
#include <QMessageBox>
#include "QStandardItemModel"
#include <iostream>
#include <sstream>
#include "QTableWidgetItem"
using namespace std;
static int save_index[DEFAULT_SIZE]={0};
static int count1=0;

QStandardItemModel* model_recv;
pkgRev::pkgRev(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pkgRev)
{
    user* currentUser = Users.getuser(Users.getCurrentNum());
    count1=0;
    cout<<currentUser->getName(*currentUser)<<endl;
    ui->setupUi(this);
    ui->name->setText(QString::fromLocal8Bit(currentUser->getName(*currentUser).data()));
    model_recv = new QStandardItemModel(ui->mainTable);
    ui->mainTable->setModel(model_recv);


    model_recv->setHorizontalHeaderItem(0, new QStandardItem("寄件人"));
    model_recv->setHorizontalHeaderItem(1, new QStandardItem("收件人"));
    model_recv->setHorizontalHeaderItem(2, new QStandardItem("寄件时间"));
    model_recv->setHorizontalHeaderItem(3, new QStandardItem("收件地址"));
    model_recv->setHorizontalHeaderItem(4, new QStandardItem("收件电话"));
    model_recv->setHorizontalHeaderItem(5, new QStandardItem("状态"));
    model_recv->setHorizontalHeaderItem(6, new QStandardItem("快递员"));
    model_recv->setHorizontalHeaderItem(7, new QStandardItem("Check"));
    ui->mainTable->setColumnWidth(0, 50);    //设置列的宽度
    ui->mainTable->setColumnWidth(1, 50);
    ui->mainTable->setColumnWidth(2, 150);
    ui->mainTable->setColumnWidth(3, 150);
    ui->mainTable->setColumnWidth(4, 150);
    ui->mainTable->setColumnWidth(5, 50);
    ui->mainTable->setColumnWidth(6, 100);
    ui->mainTable->setColumnWidth(7, 40);
    if(Packages.searchByRecver(currentUser->getName(*currentUser),*currentUser)==SUCCESS){
        cout<<currentUser<<"待收包裹查找完毕"<<endl;
        int i;

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
             if(Packages.list[Packages.checkOutcome[i]].status!=PKG_UNRECV){

             }
             else{
                 QStandardItem *checkItem = new QStandardItem();
                 checkItem->setCheckable( true );
                 checkItem->setCheckState( Qt::Unchecked );
                 list<< checkItem;
                 save_index[count1]=i;
                 count1++;
             }
             model_recv->insertRow(i, list);
        }

    }
    else {
        cout<<currentUser<<"待收包裹查找失败"<<endl;
    }

}

pkgRev::~pkgRev()
{
    delete ui;
}

void pkgRev::closeEvent(QCloseEvent *e){
    Navigation* navi = new Navigation;
    navi->showMaximized();
}
void pkgRev::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}



void pkgRev::on_Get_clicked()
{
    int i,count2=0;
    user* currentUser = Users.getuser(Users.getCurrentNum());

    for(i=0;i<count1;i++){
        if(model_recv->item(save_index[i],7)->checkState()==Qt::Checked){
            currentUser->recvPkg(Packages.list[Packages.checkOutcome[save_index[i]]]);
            count2++;
        }
    }
    stringstream ss;
    string s;
    ss<<"共收件"<<count2<<"件！点击确认退出！";
    ss>>s;
    QMessageBox::warning(this,"收件完毕！",s.data());
    this->close();
}
