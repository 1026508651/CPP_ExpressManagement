#include "adminusercheck.h"
#include "ui_adminusercheck.h"
#include "QStandardItemModel"
#include "variables.h"
#include "QMessageBox"
#include "QTableWidgetItem"
#include "admincheck.h"
#include "sstream"
#include "pkgmanregister.h"
#include "pkgmancheck.h"
static CSocket* socket=CSocket::instance();
extern int currentPkgMan;

QStandardItemModel* model_adminuser;
static int call=0;
adminUserCheck::adminUserCheck(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminUserCheck)
{
    ui->setupUi(this);

    user currentUser = Users.getuser(Users.getCurrentNum());
    ui->type->addItem("所有");
    ui->type->addItem("一般用户");
    ui->type->addItem("管理员");
    ui->type->addItem("快递员");
    ui->name->setText(QString::fromLocal8Bit(currentUser.getName(currentUser).data()));
    model_adminuser = new QStandardItemModel(ui->mainTable);
    ui->mainTable->setModel(model_adminuser);
    model_adminuser->setHorizontalHeaderItem(0, new QStandardItem("用户名"));
    model_adminuser->setHorizontalHeaderItem(1, new QStandardItem("姓名"));
    model_adminuser->setHorizontalHeaderItem(2, new QStandardItem("账户余额"));
    model_adminuser->setHorizontalHeaderItem(3, new QStandardItem("收件地址数量"));
    model_adminuser->setHorizontalHeaderItem(4, new QStandardItem("首选收件地址"));
    model_adminuser->setHorizontalHeaderItem(5, new QStandardItem("电话号码"));
    model_adminuser->setHorizontalHeaderItem(6, new QStandardItem("角色"));
    model_adminuser->setHorizontalHeaderItem(7, new QStandardItem("删除"));
    ui->mainTable->setColumnWidth(0, 100);    //设置列的宽度
    ui->mainTable->setColumnWidth(1, 70);
    ui->mainTable->setColumnWidth(2, 50);
    ui->mainTable->setColumnWidth(3, 80);
    ui->mainTable->setColumnWidth(4, 150);
    ui->mainTable->setColumnWidth(5, 100);
    ui->mainTable->setColumnWidth(6, 50);
    ui->mainTable->setColumnWidth(7, 50);
}

adminUserCheck::~adminUserCheck()
{
    delete ui;
}

void adminUserCheck::on_Get_clicked()
{
    int i;
    int currentNum=Users.getCurrentNum();
    user currentUser = Users.getuser(currentNum);
    int fail=0;
    if(ui->keyWord->text().isEmpty()){
        fail=Users.checkall(ui->type->currentIndex());
    }
    else{
        fail=Users.searchByName(string(ui->keyWord->text().toLocal8Bit()),ui->type->currentIndex());
    }
    model_adminuser->removeRows(0,model_adminuser->rowCount());
    if(fail==FAIL){
        QMessageBox::warning(this,"查找失败","查找失败！无结果！ ");
    }
    else{
        int outNumsave=Users.getOutNum();
        int *saveOutcome=Users.getOutcome();
        for(i=0;i<outNumsave;i++){
             QList<QStandardItem*> list;
             user usrSave=Users.getuser(saveOutcome[i]);
             QStandardItem *checkItem = new QStandardItem();
             checkItem->setCheckable( true );
             checkItem->setCheckState( Qt::Unchecked );
             list << new QStandardItem(QString::fromLocal8Bit(usrSave.getUsername().data()))
                     << new QStandardItem(QString::fromLocal8Bit(usrSave.getName(currentUser).data()))
                     << new QStandardItem(QString::number(usrSave.getMoney(),10))
                     << new QStandardItem(QString::number(usrSave.addressSize,10));
             if(usrSave.addressSize!=0){
                 list<< new QStandardItem(QString::fromLocal8Bit(usrSave.address[0].data()));
             }
             else{
                 list<<new QStandardItem("");
             }
             list<< new QStandardItem(QString::fromLocal8Bit(usrSave.phoneNumber.data()));
             list<< new QStandardItem(QString::fromLocal8Bit(usrSave.getCharacter().data()));
             if(usrSave.character!=ADMIN_USER){
                 QStandardItem *checkItem = new QStandardItem();
                 checkItem->setCheckable( true );
                 checkItem->setCheckState( Qt::Unchecked );
                 list<< checkItem;
             }
             model_adminuser->insertRow(i, list);
        }
        QMessageBox::warning(this,"查找完毕","查找完毕！ ");
        delete[] saveOutcome;
    }
}
void adminUserCheck::closeEvent(QCloseEvent *e){
    if(call==0){
        currentPkgMan=FAIL;
        AdminCheck* admin_navi = new AdminCheck;
        admin_navi->showMaximized();
    }
    else call=0;
}
void adminUserCheck::resizeEvent(QResizeEvent* event){
    QMainWindow::resizeEvent(event);
    int width = ui->centralwidget->width(), height = ui->centralwidget->height();
       ui->mainFrame->move(width/2-400, height/3-150);
    // Your code here
}

void adminUserCheck::on_delete_2_clicked()
{
    int i=0,count=0;

    int *saveOutcome=Users.getOutcome();

    for(i=0;i<model_adminuser->rowCount();i++){
        user usrSave=Users.getuser(saveOutcome[i]);
        if(usrSave.character!=ADMIN_USER){
            if(model_adminuser->item(i,7)->checkState()==Qt::Checked){
                Users.userDelete(saveOutcome[i]);
                count++;
            }
        }
    }
    stringstream ss;
    string s;
    ss<<"共删除"<<count<<"个用户！";
    ss>>s;
    QMessageBox::warning(this,"删除成功！",s.data());
    ui->Get->click();
}

void adminUserCheck::on_addPkgMan_clicked()
{
    pkgManRegister* reg=new pkgManRegister;
    reg->showMaximized();
    call=1;
    this->close();
}

void adminUserCheck::on_mainTable_doubleClicked(const QModelIndex &index)
{
    int outNumsave=Users.getOutNum();
    int *saveOutcome=Users.getOutcome();

    if(Users.getuser(saveOutcome[index.row()]).character==PACKAGE_USER){
        currentPkgMan=saveOutcome[index.row()];
        call=1;
        pkgManCheck* pack=new pkgManCheck;
        pack->showMaximized();
        this->close();
    }
}
