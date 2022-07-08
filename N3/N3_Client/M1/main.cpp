#include "variables.h"
#include "login.h"
#include <QTextCodec>
#include "sstream"
#include <QApplication>
#include <math.h>
#include <QWebSocket>
#include <QJsonArray>
#include <QJsonDocument>
static unsigned int Time = 0;
userList Users;
packageList Packages;
//客户端main相比之前改动如下：
//所有函数仅做转发，由于C++没有实现序列化功能，因此需要手写添加参数和函数调用，将各个函数按顺序编号，对应不同类
//char userLocal[DEFAULT_SIZE] = "users.txt";
//char packageLocal[DEFAULT_SIZE] = "packages.txt";
//userList Users = userList(userLocal);
//packageList Packages = packageList(packageLocal);
extern QJsonObject recvBuffer; //接收缓冲区
extern int classType;//函数调用类种类
extern int funcNum;//函数调用函数序号
extern int recved;//流量控制 是否接收
extern int sendable;//流量控制 是否可以发送
static CSocket* socket=CSocket::instance();//多页面公用同一个Socket
extern int isFirst;
int isFirst=0;
QJsonObject recvBuffer;
int classType;
int funcNum;
int recved;
int sendable;
string user::getUsername(){
    return this->username;
}
user::user() {
    character = NORMAL_USER;
    username = "";
    name = "";
    int i;
    for (i = 0; i < DEFAULT_SIZE; i++) {
        address[i] = "";
    }
    addressSize = 0;
    password = "";
    money = 0;
    state = 0;
}
user::user(int ids, int characters, string usernames, string names, int adressSizes, string adress[], string passwords, string phoneNumbers, int moneys, int states,int visible) {
    id = ids;
    character = characters;
    username = usernames;
    name = names;
    int i;
    for (i = 0; i < adressSizes; i++) {
        address[i] = adress[i];
    }
    phoneNumber=phoneNumbers;
    addressSize = adressSizes;
    password = passwords;
    money = moneys;
    state = states;
    this->visible=visible;
}
user::user(int ids, string usernames, string names, string passwords, string phoneNumbers,int type) {
    id = ids;
    username = usernames;
    name = names;
    password = passwords;
    phoneNumber = phoneNumbers;
    character = type;
    addressSize = 0;
    money = 0;
    state = USER_UNLOGIN;
    visible=VISABLE;
}
user::user(QJsonObject* paras){
    this->username=string(paras->value("username").toString().toLocal8Bit());
    this->name=string(paras->value("name").toString().toLocal8Bit());
    this->password=string(paras->value("password").toString().toLocal8Bit());
    this->money=paras->value("money").toString().toInt();
    this->id=paras->value("id").toString().toInt();
    this->addressSize=paras->value("addressSize").toString().toInt();
    int i;
    for(i=0;i<addressSize;i++){
        this->address[i]=string(paras->value("address").toArray()[i].toString().toLocal8Bit());
    }
    this->visible=paras->value("visible").toString().toInt();
    this->phoneNumber=string(paras->value("phoneNumber").toString().toLocal8Bit());
    this->state= paras->value("state").toString().toInt();
    this->character= paras->value("character").toString().toInt();
}
int user::packgeUsrSendPkg(int i){
    Packages.pkgManSendPkg(i);
    return SUCCESS;
}
string user::getCharacter(){
    switch(this->character){
        case NORMAL_USER:
            return string("用户");
        case ADMIN_USER:
            return string("管理员");
        case PACKAGE_USER:
            return string("快递员");
    }
    return "";
}
int user::getMoney() {
    return money;
}
int user::addMoney(int num) {
    money += num;
    updateUser(*this);
    return money;
}
int user::updateUser(user usr){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("usr",usr.toJsonObject());

    funcSave.insert("classType",3);
    funcSave.insert("funcNum",1);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=3;
    funcNum=1;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QJsonObject save=recvBuffer;
    recved=FAIL;

}
int user::recvPkg(int pkgNum) {
    if(this->character==NORMAL_USER){
        Packages.packageRecv(pkgNum);
        return SUCCESS;
    }
    else return FAIL;
}
int user::checkRepeat(string newUsername) {
    return username.compare(newUsername);
}
int user::checkRepeat(user temp) {
    if(username.size()==temp.username.size()&&temp.password.size()==password.size()){
        if(username.compare(temp.username)==0&&password.compare(temp.password)==0)
        return 0;
        else return -1;
    }
    else return -1;
}
int user::changePasswd(string temp){
    this->password=temp;
    updateUser(*this);
    return SUCCESS;
}
int user::compareName(string otherName){
    return name.compare(otherName);
}

int user::checkPasswd(string temp){
    return temp.compare(this->password);
}
string user::getName(user& usr){
    if(state==USER_LOGGED||usr.character!=NORMAL_USER)return name;
    else return "";
}
int user::addAdress(string newAdress) {
    if(this->character==NORMAL_USER){
        if (addressSize < DEFAULT_SIZE) {
            address[addressSize] = newAdress;
            addressSize++;
            updateUser(*this);
            return SUCCESS;
        }
        else return FAIL;
    }

    else return FAIL;}
long long user::sendPkg(long long pkgNum/*??????*/, int num/*?????????????????0*/,string recvName,int recvNum,string recvPhone,string itemDes,int type,int unit) {
    if (state == USER_LOGGED&&this->character==NORMAL_USER) {
        //??????????
        package pkg = package(pkgNum,type,unit);
        if(pkg.getPrice()>this->getMoney()){
            return FAIL;
        }
        pkg.packWrite(recvName, recvPhone, address[recvNum], itemDes);
        pkg.packWrite(name, address[num]);
        Packages.add(pkg);
        Users.payMoney(pkg.getPrice(),this->id,Users.getAdmin());
        Users.getuser(Users.getAdmin()).addMoney(pkg.getPrice());

        return pkgNum;
    }
    else return FAIL;
}
QJsonObject user::toJsonObject(){
    QJsonObject usrOb;
    usrOb.insert("username",QString::fromLocal8Bit(username.data()));
    usrOb.insert("name",QString::fromLocal8Bit(name.data()));
    usrOb.insert("password",QString::fromLocal8Bit(password.data()));
    QJsonArray adress;
    usrOb.insert("money",QString::number(money));
    usrOb.insert("id",QString::number(id));
    usrOb.insert("state",QString::number(state));
    usrOb.insert("character",QString::number(character));
    usrOb.insert("addressSize",QString::number(addressSize));
    usrOb.insert("visible",QString::number(visible));
    usrOb.insert("phoneNumber",QString::fromLocal8Bit(phoneNumber.data()));
    int i;
    for(i=0;i<addressSize;i++){
        adress.append(QString::fromLocal8Bit(address[i].data()));
    }
    usrOb.insert("address",adress);
    return usrOb;
}

package::package(long long pkgNums,int type,int unit){
    pkgNum=pkgNums;
    status=PKG_INIT;
    sender="";
    recvPhone="";
    sendAdress="";
    recvAdress="";
    recver="";
    sendTime=0;
    recvTime=0;
    itemDes="";
    switch(type){
    case TYPE_NORMAL:
        this->item=new normalItem;

        break;
    case TYPE_GLASS:
        this->item=new glassItem;
        break;
    case TYPE_BOOK:
        this->item=new bookItem;
        break;
    }
    this->item->setType(type);
    this->item->unit=unit;
}
package::package() {
    pkgNum = 0;
    status = 0;
    sender = "";
    recvPhone = "";
    sendAdress = "";
    recvAdress = "";
    recver = "";
    sendTime = 0;
    recvTime = 0;
    itemDes = "";
}
package::package(int packageMan,long long pkgNum_save, int status_save, string sender_save, string recvPhone_save, string sendAdress_save, string recvAdress_save, string recver_save, unsigned int sendTime_save, unsigned int recvTime_save, string itemDes_save,basicItem* itemSave) {
    pkgNum = pkgNum_save;
    status = status_save;
    sender = sender_save;
    recvPhone = recvPhone_save;
    sendAdress = sendAdress_save;
    recvAdress = recvAdress_save;
    recver = recver_save;
    sendTime = sendTime_save;
    recvTime = recvTime_save;
    itemDes = itemDes_save;
    item=itemSave;
    this->packageMan=packageMan;
}
int package::getPkgMan()
{
    return this->packageMan;
}
int package::recved() {
    status = PKG_RECV;
    time_t t;
    struct tm *tmp;
    time(&t);
    tmp = localtime(&t);
    //2204301710
    Time = tmp->tm_year % 100 * 100000000 + (tmp->tm_mon + 1) * 1000000 + (tmp->tm_mday) * 10000 + tmp->tm_hour * 100 + tmp->tm_min;
    recvTime = Time;
    Users.getuser(packageMan).addMoney((int)floor(item->getPrice()*0.5));//???????????? ???????
    Users.getuser(Users.getAdmin()).addMoney(-(int)floor(item->getPrice()*0.5));//???????????? ???????
    return SUCCESS;
}
int package::senderCompare(string someone){
    return sender.compare(someone);
}
int package::pkgManCompare(int man){
    if(man==this->packageMan)return 0;
    else return FAIL;
}
int package::recverCompare(string someone) {
    return recver.compare(someone);
}
int package::packWrite(const string& recvName,string recvPhones,const string& recvAddress,const string& itemDess){
    sender="";
    recvPhone=recvPhones;
    sendAdress="";
    recvAdress=recvAddress;
    recver=recvName;
    time_t t;
    struct tm *tmp;
    time(&t);
    tmp = localtime(&t);
    //2204301710
    Time= tmp->tm_year%100*100000000+(tmp->tm_mon+1)*1000000+(tmp->tm_mday)*10000+tmp->tm_hour*100+tmp->tm_min;
    sendTime=Time;
    recvTime=0;
    itemDes=itemDess;
    return status;
}
int package::getPrice(){
    return this->item->getPrice();
}
int package::packWrite(const string& senders,const string& sendAddress){
    sender=senders;
    sendAdress=sendAddress;
    this->packageMan=Users.pkgManQinDing();
    status=PKG_WAITSEND;
    return status;
}

string package::getStatus(){
    string s;
    switch(this->status){
        case PKG_INIT:
        s=string("初始化");
        break;
    case PKG_WAITSEND:
        s=string("待揽收");
        break;
    case PKG_RECV:
        s=string("已签收");
        break;
    case PKG_UNRECV:
        s=string("运输中");
        break;
    }
    return s;

}
string package::getRecver(){
    return recver;
}
string package::getSender(){
    return sender;
}
string package::getRecvPhone(){
    return recvPhone;
}
string package::getRecvAdress(){
    return recvAdress;
}
QJsonObject package::toJsonObject(){
     QJsonObject pkgOb;
     pkgOb.insert("packageMan",QString::number(packageMan));
     pkgOb.insert("sender",QString::fromLocal8Bit(sender.data()));
     pkgOb.insert("recvPhone",QString::fromLocal8Bit(recvPhone.data()));
     pkgOb.insert("sendAdress",QString::fromLocal8Bit(sendAdress.data()));
     pkgOb.insert("recvAdress",QString::fromLocal8Bit(recvAdress.data()));
     pkgOb.insert("recver",QString::fromLocal8Bit(recver.data()));
     pkgOb.insert("itemDes",QString::fromLocal8Bit(itemDes.data()));
     pkgOb.insert("itemType",QString::number(item->type));
     pkgOb.insert("status",QString::number(status));
     pkgOb.insert("pkgNum",QString::number(pkgNum));
     pkgOb.insert("sendTime",QString::number(sendTime));
     pkgOb.insert("recvTime",QString::number(recvTime));
     pkgOb.insert("itemUnit",QString::number(item->unit));
     return pkgOb;

}
package::package(QJsonObject paras){
    this->packageMan=paras.value("packageMan").toString().toInt();
    this->sender=string(paras.value("sender").toString().toLocal8Bit());
    this->recvPhone=string(paras.value("recvPhone").toString().toLocal8Bit());
    this->sendAdress=string(paras.value("sendAdress").toString().toLocal8Bit());
    this->recvAdress=string(paras.value("recvAdress").toString().toLocal8Bit());
    this->recver=string(paras.value("recver").toString().toLocal8Bit());
    this->itemDes=string(paras.value("itemDes").toString().toLocal8Bit());
    int itemType=paras.value("itemType").toString().toInt();
    switch(itemType){
    case TYPE_GLASS:
        this->item=new glassItem;
        break;
    case TYPE_BOOK:
        this->item=new bookItem;
        break;
    case TYPE_NORMAL:
        this->item=new normalItem;
        break;
    }
    this->item->unit=paras.value("itemUnit").toString().toInt();
    this->item->setType(itemType);
    this->status=paras.value("status").toString().toInt();
    this->pkgNum=paras.value("pkgNum").toString().toLongLong();
    this->sendTime=paras.value("sendTime").toString().toUInt();
    this->recvTime=paras.value("recvTime").toString().toUInt();

}

package packageList::getPkg(int num){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("num",num);
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",1);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));


    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=1;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    QJsonValue arraySave=recvBuffer.value("return");
    QJsonObject objectSave=arraySave.toObject();

        package value=package(objectSave);

        return value;


}
int packageList::getCheckSize(){
    QJsonObject funcSave;
    QJsonObject paras;
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",2);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));

    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=2;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    return save.value("return").toInt();
}
int packageList::add(package& newpack){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("newpack",newpack.toJsonObject());
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",3);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=3;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    return save.value("return").toInt();
}
int packageList::searchRecverBySender(string name,user& usr,string keyname){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("name",QString::fromLocal8Bit(name.data()));
    paras.insert("id",usr.id);
    paras.insert("keyWord",QString::fromLocal8Bit(keyname.data()));

    funcSave.insert("classType",1);
    funcSave.insert("funcNum",4);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=4;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    return save.value("return").toInt();
}
int packageList::searchSenderByRecver(string name, user& usr,string keyname) {
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("name",QString::fromLocal8Bit(name.data()));
    paras.insert("id",usr.id);
    paras.insert("keyWord",QString::fromLocal8Bit(keyname.data()));

    funcSave.insert("classType",1);
    funcSave.insert("funcNum",5);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=5;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    return save.value("return").toInt();
}
int packageList::searchByRecver(string name, user& usr) {
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("name",QString::fromLocal8Bit(name.data()));
    paras.insert("usr",usr.toJsonObject());
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",6);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=6;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    return save.value("return").toInt();
}
int packageList::searchByTime(unsigned int down, unsigned int up, int type, user& usr) {
    //type:1??????? 2???????
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("down",QString::number(down));
    paras.insert("up",QString::number(up));
    paras.insert("type",type);
    paras.insert("usr",usr.toJsonObject());
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",7);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=7;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    return save.value("return").toInt();
}
int packageList::searchByPackgeMan(string name, user &usr){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("name",QString::fromLocal8Bit(name.data()));
    paras.insert("usr",usr.toJsonObject());
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",8);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=8;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    return save.value("return").toInt();
}
int packageList::packageRecv(int pkgNum){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("pkgNum",QString::number(pkgNum));
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",9);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=9;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    return save.value("return").toInt();
}

int* packageList::getCheckOutcom(){
    QJsonObject funcSave;
    QJsonObject paras;
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",10);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=10;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    QJsonValue arraySave=recvBuffer.value("return");
    QJsonObject objectSave=arraySave.toObject();
    int size=objectSave.size();
    int* outcome;
    QString number;
    outcome=new int[size];
    for(i=0;i<size;i++){
        number=QString::number(i);
        outcome[i]=objectSave.value(number).toString().toInt();
    }

    return outcome ;
}
int packageList::getSize(){
    QJsonObject funcSave;
    QJsonObject paras;
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",11);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=11;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    int size=save.value("return").toInt();
    return size;
}
int packageList::setOutcome(int pkgNum){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("pkgNum",pkgNum);
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",12);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=12;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    int size=save.value("return").toInt();
    return size;
}
int packageList::pkgManSendPkg(int pkgNum){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("pkgNum",pkgNum);
    funcSave.insert("classType",1);
    funcSave.insert("funcNum",13);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=1;
    funcNum=13;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    int size=save.value("return").toInt();
    return size;
}
int packageList::search(int searchID, QString keyWord, int currentUser){
    int returnValue=0;
    QStringList savePara;
    user currentUsername=Users.getuser(currentUser);
    switch(searchID){
    case 0:
        returnValue=Packages.searchRecverBySender(currentUsername.getName(currentUsername),currentUsername,string(keyWord.toLocal8Bit()));
        break;
    case 1:
        returnValue=Packages.searchSenderByRecver(currentUsername.getName(currentUsername),currentUsername,string(keyWord.toLocal8Bit()));
        break;
    case 2:
    case 3:
        savePara=keyWord.split("|");
        returnValue=Packages.searchByTime(savePara[0].toUInt(),savePara[0].toUInt(),savePara[1].toInt(),currentUsername);
        break;
    case 4:
        returnValue=Packages.setOutcome(keyWord.toInt());
        break;
    case 5:
        returnValue=Packages.searchByRecver(string(keyWord.toLocal8Bit()),currentUsername);
        break;
    }
    return returnValue;
}


int userList::payMoney(int num, int fromUsr, int toUsr){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("num",num);
    paras.insert("fromUsr",fromUsr);
    paras.insert("toUsr",toUsr);

    funcSave.insert("classType",2);
    funcSave.insert("funcNum",1);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=1;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    int size=save.value("return").toInt();
    return size;
}
user userList::getuser(int id){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("id",id);

    funcSave.insert("classType",2);
    funcSave.insert("funcNum",2);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=2;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    QJsonValue arraySave=recvBuffer.value("return");
    QJsonObject objectSave=arraySave.toObject();
    user saveUsr=user(&objectSave);
    return saveUsr;
}
int userList::getSize(){
    QJsonObject funcSave;
    QJsonObject paras;

    funcSave.insert("classType",2);
    funcSave.insert("funcNum",3);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=3;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    int size=recvBuffer.value("return").toInt();
    return size;
}
int userList::registerUser(string username, string name, string password, string phoneNumber,int type) {
    //????
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("username",QString::fromLocal8Bit(username.data()));
    paras.insert("name",QString::fromLocal8Bit(name.data()));
    paras.insert("password",QString::fromLocal8Bit(password.data()));
    paras.insert("phoneNumber",QString::fromLocal8Bit(phoneNumber.data()));
    paras.insert("type",type);
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",4);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=4;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    int size=recvBuffer.value("return").toInt();
    return size;
}
int userList::getOutNum(){
    QJsonObject funcSave;
    QJsonObject paras;
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",5);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=5;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    int size=recvBuffer.value("return").toInt();
    return size;
}
int* userList::getOutcome(){
    QJsonObject funcSave;
    QJsonObject paras;
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",6);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=6;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    QJsonObject recv=recvBuffer.value("return").toObject();
    int size=recv.size();
    int* outcome=new int[size];
    for(i=0;i<size;i++){
        outcome[i]=recv.value(QString::number(i)).toString().toInt();
    }
    return outcome;



}

int userList::checkall(int type){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("type",type);
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",7);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=7;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;

    int size=recvBuffer.value("return").toInt();

    return size;

}
int userList::searchByName(string name,int type){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("name",QString::fromLocal8Bit(name.data()));
    paras.insert("type",type);
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",8);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=8;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;

    int size=recvBuffer.value("return").toInt();

    return size;

}
int userList::login(user temp) {//???????ID??userList???λ?????????Fail
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("temp",temp.toJsonObject());
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",9);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);

    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=9;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;

    int size=recvBuffer.value("return").toInt();

    return size;


}
int userList::setCurrentNum(int num){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("num",num);
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",10);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=10;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;

    int size=recvBuffer.value("return").toInt();

    return size;
}
int userList::getCurrentNum(){
    QJsonObject funcSave;
    QJsonObject paras;
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",11);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=11;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;

    int size=recvBuffer.value("return").toInt();

    return size;
}
int userList::getAdmin(){
    QJsonObject funcSave;
    QJsonObject paras;
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",12);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=12;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;

    int size=recvBuffer.value("return").toInt();

    return size;
}
int userList::userDelete(int num){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("num",num);
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",13);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=13;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;

    int size=recvBuffer.value("return").toInt();

    return size;

}
int userList::pkgManQinDing(){
    QJsonObject funcSave;
    QJsonObject paras;
    funcSave.insert("classType",2);
    funcSave.insert("funcNum",14);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=2;
    funcNum=14;
    socket->onSendMesssage(strRet);
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();

    QJsonObject save=recvBuffer;
    recved=FAIL;
    int size=recvBuffer.value("return").toInt();
    return size;

}
int userList::userLogout(int id){
    QJsonObject funcSave;
    QJsonObject paras;
    paras.insert("id",id);
    funcSave.insert("classType",3);
    funcSave.insert("funcNum",2);
    funcSave.insert("paras",paras);
    QString strRet;
    QJsonDocument doc(funcSave);
    strRet = QString(doc.toJson(QJsonDocument::Compact));
    int i;
    while(sendable==SUCCESS||classType!=FAIL||funcNum!=FAIL){
        i=0;
    }
    classType=3;
    funcNum=2;
    socket->onSendMesssage(strRet);
    QJsonObject save=recvBuffer;
    QEventLoop eventLoop;
    QObject::connect(socket, SIGNAL(endRecieved()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    recved=FAIL;
    int size=recvBuffer.value("return").toInt();

    return size;
}
int basicItem::setType(int type){
    this->type=type;
    return SUCCESS;
}

//???????????????????","
string timeconvert(unsigned int time){
    int year=2000+time/100000000;
    time%=100000000;
    int month=time/1000000;
    time%=1000000;
    int day=time/10000;
    time%=10000;
    int hour=time/100;
    time%=100;
    int minute=time;
    stringstream ss;
    string s;
    ss << year<<'-'<<month<<'-'<<day<<' '<<hour<<':'<<minute;
    ss >> s;
    return s;

}
int bookItem::getPrice(){
    return this->unit*this->perPrice;
}
int glassItem::getPrice(){
    return this->unit*this->perPrice;
}
int normalItem::getPrice(){
    return this->unit*this->perPrice;
}
int main(int argc, char *argv[])
{


    classType=FAIL;
    funcNum=FAIL;
    sendable=FAIL;
    recved=FAIL;
    QApplication a(argc, argv);
    string str1="@Zhengxun He 书写于2022-05-25";
    QString qss=QString::fromLocal8Bit(str1.data());
    QByteArray mm=qss.toLocal8Bit();
    string str=string(mm);
    cout<<str<<endl;
    login w;
    w.showMaximized();
    return a.exec();
}
