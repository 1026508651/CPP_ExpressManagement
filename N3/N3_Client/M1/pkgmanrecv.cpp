#include "pkgmanrecv.h"
#include "ui_pkgmanrecv.h"
#include "variables.h"
#include "QStandardItem"
#include "QTableWidgetItem"
#include "sstream"
#include "QMessageBox"
#include "pkgmannavi.h"
static QStandardItemModel* model_send;
static int save_index[DEFAULT_SIZE]={0};
static int count1=0;
static CSocket* socket=CSocket::instance();
pkgManRecv::pkgManRecv(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pkgManRecv)
{

    count1=0;
    ui->setupUi(this);

    int userNum=Users.getCurrentNum();
    user currentUser = Users.getuser(userNum);
    ui->name->setText(QString::fromLocal8Bit(currentUser.getName(currentUser).data()));
    model_send = new QStandardItemModel(ui->mainTable);
    ui->mainTable->setModel(model_send);
    QStandardItem *checkItem = new QStandardItem();
    checkItem->setCheckable( true );
    checkItem->setCheckState( Qt::Unchecked );

    model_send->setHorizontalHeaderItem(0, new QStandardItem("寄件人"));
    model_send->setHorizontalHeaderItem(1, new QStandardItem("收件人"));
    model_send->setHorizontalHeaderItem(2, new QStandardItem("寄件时间"));
    model_send->setHorizontalHeaderItem(3, new QStandardItem("收件地址"));
    model_send->setHorizontalHeaderItem(4, new QStandardItem("收件电话"));
    model_send->setHorizontalHeaderItem(5, new QStandardItem("状态"));
    model_send->setHorizontalHeaderItem(6, new QStandardItem("快递员"));
    model_send->setHorizontalHeaderItem(7, new QStandardItem("Check"));
    ui->mainTable->setColumnWidth(0, 50);    //设置列的宽度
    ui->mainTable->setColumnWidth(1, 50);
    ui->mainTable->setColumnWidth(2, 120);
    ui->mainTable->setColumnWidth(3, 120);
    ui->mainTable->setColumnWidth(4, 150);
    ui->mainTable->setColumnWidth(5, 50);
    ui->mainTable->setColumnWidth(6, 50);
    ui->mainTable->setColumnWidth(7, 40);
    if(Packages.search(SEARCH_BY_PKGMAN,QString::fromLocal8Bit(currentUser.getName(currentUser).data()),userNum)==SUCCESS){
        cout<<"待收包裹查找完毕"<<endl;
        int i;
        int saveCheckSize=Packages.getCheckSize();
        int* saveOutcome=Packages.getCheckOutcom();
        for(i=0;i<saveCheckSize;i++){
             QList<QStandardItem*> list;
             QTableWidgetItem* p_check = new QTableWidgetItem();
             p_check->setCheckState(Qt::Unchecked);
             package savePkg=Packages.getPkg(saveOutcome[i]);
             list << new QStandardItem(QString::fromLocal8Bit(savePkg.getSender().data()))
                     << new QStandardItem(QString::fromLocal8Bit(savePkg.getRecver().data()))
                     << new QStandardItem(QString::fromLocal8Bit(timeconvert(savePkg.sendTime).data()))
                     << new QStandardItem(QString::fromLocal8Bit(savePkg.getRecvAdress().data()))
                     << new QStandardItem(QString::fromLocal8Bit(savePkg.getRecvPhone().data()))
                     << new QStandardItem(QString::fromLocal8Bit(savePkg.getStatus().data()));
                    user savePkgMan=Users.getuser(savePkg.getPkgMan());
                     list<< new QStandardItem(QString::fromLocal8Bit(savePkgMan.getName(savePkgMan).data()));
             if(savePkg.status!=PKG_WAITSEND){

             }
             else{
                 QStandardItem *checkItem = new QStandardItem();
                 checkItem->setCheckable( true );
                 checkItem->setCheckState( Qt::Unchecked );
                 list<< checkItem;
                 save_index[count1]=i;
                 count1++;
             }
             model_send->insertRow(i, list);
        }

    }
    else {
        cout<<"待收包裹查找失败"<<endl;
    }

}

pkgManRecv::~pkgManRecv()
{
    delete ui;
}
void pkgManRecv::closeEvent(QCloseEvent *e){
    pkgManNavi *navi=new pkgManNavi;
    navi->showMaximized();

}
void pkgManRecv::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   int width = ui->centralwidget->width(), height = ui->centralwidget->height();
      ui->mainFrame->move(width/2-400, height/3-150);
   // Your code here
}

void pkgManRecv::on_Get_clicked()
{
    int i;
    user currentUser = Users.getuser(Users.getCurrentNum());
    int saveCheckSize=Packages.getCheckSize();
    int* saveOutcome=Packages.getCheckOutcom();
    for(i=0;i<count1;i++){
        if(model_send->item(save_index[i],7)->checkState()==Qt::Checked){
            currentUser.packgeUsrSendPkg(saveOutcome[save_index[i]]);
        }
    }
    stringstream ss;
    string s;
    ss<<"共揽收"<<count1<<"件！点击确认退出！";
    ss>>s;
    QMessageBox::warning(this,"揽收完毕！",s.data());
    this->close();
}
