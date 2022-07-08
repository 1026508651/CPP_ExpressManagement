#include "adminusercheck.h"
#include "ui_adminusercheck.h"
#include "QStandardItemModel"
#include "variables.h"
#include "QMessageBox"
#include "QTableWidgetItem"
#include "admincheck.h"
static QStandardItemModel* model_adminuser;
adminUserCheck::adminUserCheck(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminUserCheck)
{
    ui->setupUi(this);
    //获取当前用户
    user* currentUser = Users.getuser(Users.getCurrentNum());
    //初始化表格
    ui->name->setText(QString::fromLocal8Bit(currentUser->getName().data()));
    model_adminuser = new QStandardItemModel(ui->mainTable);
    ui->mainTable->setModel(model_adminuser);
    model_adminuser->setHorizontalHeaderItem(0, new QStandardItem("用户名"));
    model_adminuser->setHorizontalHeaderItem(1, new QStandardItem("姓名"));
    model_adminuser->setHorizontalHeaderItem(2, new QStandardItem("账户余额"));
    model_adminuser->setHorizontalHeaderItem(3, new QStandardItem("收件地址数量"));
    model_adminuser->setHorizontalHeaderItem(4, new QStandardItem("首选收件地址"));
    model_adminuser->setHorizontalHeaderItem(5, new QStandardItem("电话号码"));
    model_adminuser->setHorizontalHeaderItem(6, new QStandardItem("角色"));
    ui->mainTable->setColumnWidth(0, 100);    //设置列的宽度
    ui->mainTable->setColumnWidth(1, 70);
    ui->mainTable->setColumnWidth(2, 50);
    ui->mainTable->setColumnWidth(3, 80);
    ui->mainTable->setColumnWidth(4, 150);
    ui->mainTable->setColumnWidth(5, 100);
    ui->mainTable->setColumnWidth(6, 50);
}

adminUserCheck::~adminUserCheck()
{
    delete ui;
}

void adminUserCheck::on_Get_clicked()
{
    int i;
    int fail=0;
    //按姓名搜索 关键词为空则直接all
    if(ui->keyWord->text().isEmpty()){
        fail=Users.checkall();
    }
    else{
        fail=Users.searchByName(string(ui->keyWord->text().toLocal8Bit()));
    }
    model_adminuser->removeRows(0,model_adminuser->rowCount());
    if(fail==FAIL){
        QMessageBox::warning(this,"查找失败","查找失败！无结果！ ");
    }
    else{
        //前端内容展示
        for(i=0;i<Users.outNum;i++){
             QList<QStandardItem*> list;
             QTableWidgetItem* p_check = new QTableWidgetItem();
             p_check->setCheckState(Qt::Unchecked);
             list << new QStandardItem(QString::fromLocal8Bit(Users.list[Users.output[i]].getUsername().data()))
                     << new QStandardItem(QString::fromLocal8Bit(Users.list[Users.output[i]].getName().data()))
                     << new QStandardItem(QString::number(Users.list[Users.output[i]].getMoney(),10))
                     << new QStandardItem(QString::number(Users.list[Users.output[i]].addressSize,10));
             if(Users.list[Users.output[i]].addressSize!=0){
                 list<< new QStandardItem(QString::fromLocal8Bit(Users.list[Users.output[i]].address[0].data()));
             }
             else{
                 list<<new QStandardItem("");
             }
             list<< new QStandardItem(QString::fromLocal8Bit(Users.list[Users.output[i]].phoneNumber.data()));
             list<< new QStandardItem(QString::fromLocal8Bit(Users.list[Users.output[i]].getCharacter().data()));
             model_adminuser->insertRow(i, list);
        }
        QMessageBox::warning(this,"查找完毕","查找完毕！ ");
    }
}
void adminUserCheck::closeEvent(QCloseEvent *e){
    AdminCheck* admin_navi = new AdminCheck;
    admin_navi->showMaximized();
}
void adminUserCheck::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    int width = ui->centralwidget->width(), height = ui->centralwidget->height();
       ui->mainFrame->move(width/2-400, height/3-150);
    // Your code here
}
