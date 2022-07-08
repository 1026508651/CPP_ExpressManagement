
#include "variables.h"
#include "serverface.h"
#include <QTextCodec>
#include "sstream"
#include <QApplication>
#include "QJsonArray"
#include <math.h>
static unsigned int Time = 0;
char userLocal[DEFAULT_SIZE] = "users.txt";
char packageLocal[DEFAULT_SIZE] = "packages.txt";
userList Users = userList(userLocal);
packageList Packages = packageList(packageLocal);
extern QString recvBuffer;

string user::getUsername(){
    return this->username;
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
int user::packgeUsrSendPkg(int i){
    Packages.pkgManSendPkg(i);
    return SUCCESS;
}
string user::getCharacter(){
    switch(this->character){
        case NORMAL_USER:
            return string("�û�");
        case ADMIN_USER:
            return string("����Ա");
        case PACKAGE_USER:
            return string("����Ա");
    }
    return "";
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
int user::getMoney() {
    return money;
}
int user::addMoney(int num) {
    money += num;
    return money;
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
    return SUCCESS;
}
int user::compareName(string otherName){
    return name.compare(otherName);
}
int user::printData(ofstream& ofs) {
    ofs << id << ',' << character << ',' << username << ',' << name << ',' << addressSize << ',';
    int i;
    for (i = 0; i < addressSize; i++) {
        ofs << address[i] << ',';
    }
    ofs << password << ',' << phoneNumber << ',' << money << ',' << state<<","<<visible << endl;
    return SUCCESS;
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
            return SUCCESS;
        }
        else return FAIL;
    }

    else return FAIL;}
long long user::sendPkg(long long pkgNum, int num,string recvName,int recvNum,string recvPhone,string itemDes,int type,int unit) {
    if (state == USER_LOGGED&&this->character==NORMAL_USER) {
        package pkg = package(pkgNum,type,unit);
        if(pkg.getPrice()>this->getMoney()){
            return FAIL;
        }
        pkg.packWrite(recvName, recvPhone, address[recvNum], itemDes);
        pkg.packWrite(name, address[num]);
        Packages.add(pkg);
        Users.payMoney(pkg.getPrice(),this->id,Users.getAdmin());
        Users.getuser(Users.getAdmin())->addMoney(pkg.getPrice());

        return pkgNum;
    }
    else return FAIL;
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
    Users.getuser(packageMan)->addMoney((int)floor(item->getPrice()*0.5));//���ݷ�ȫ������ ����ȡ��
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
int package::printData(ofstream& ofs){
    ofs<<pkgNum<<','<<status<<','<<sender<<','<<recvPhone<<','<<sendAdress<<','<<recvAdress<<','<<recver<<','<<sendTime<<','<<recvTime<<','<<itemDes<<","<<packageMan<<",";
    this->item->printData(ofs);
    return SUCCESS;
}
string package::getStatus(){
    string s;
    switch(this->status){
        case PKG_INIT:
        s=string("��д��");
        break;
    case PKG_WAITSEND:
        s=string("������");
        break;
    case PKG_RECV:
        s=string("��ǩ��");
        break;
    case PKG_UNRECV:
        s=string("������");
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
    this->packageMan=paras.value("packageMan").toInt();
    this->sender=string(paras.value("sender").toString().toLocal8Bit());
    this->recvPhone=string(paras.value("recvPhone").toString().toLocal8Bit());
    this->sendAdress=string(paras.value("sendAdress").toString().toLocal8Bit());
    this->recvAdress=string(paras.value("recvAdress").toString().toLocal8Bit());
    this->recver=string(paras.value("recver").toString().toLocal8Bit());
    this->itemDes=string(paras.value("itemDes").toString().toLocal8Bit());
    int itemType=paras.value("itemType").toInt();
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
    this->item->unit=paras.value("itemUnit").toInt();
    this->item->setType(itemType);
    this->status=paras.value("status").toInt();
    this->pkgNum=paras.value("pkgNum").toString().toLongLong();
    this->sendTime=paras.value("sendTime").toString().toUInt();
    this->recvTime=paras.value("recvTime").toString().toUInt();

}

package packageList::getPkg(int num){
    return list[num];
}
int packageList::getCheckSize(){
    return checkSize;
}
int packageList::add(package& newpack){
    newpack.pkgNum=size;
    package *newList;
    newList=new package[size+1];
    size++;
    int i;
    for(i=0;i<size-1;i++){
        newList[i]=list[i];
    }
    delete []list;
    newList[i]=newpack;
    list=newList;
    return SUCCESS;
}
int packageList::searchRecverBySender(string name,user& usr,string keyname){
    if(keyname=="*"){
        this->checkSize=size;
        delete [] this->checkOutcome;
        checkOutcome=new int[size];
        int i;
        for(i=0;i<size;i++){
            checkOutcome[i]=i;
        }
        return SUCCESS;
    }
    if(usr.character==ADMIN_USER||(usr.character==NORMAL_USER&&name.compare(usr.getName(usr))==0)||usr.character==PACKAGE_USER){
        if((usr.character==NORMAL_USER&&name.compare(usr.getName(usr))==0)){
            int i,count;
            count=0;
            for(i=0;i<size;i++){
                if((list[i].senderCompare(name)==0&&list[i].recverCompare(keyname)==0)||(name.compare(keyname)==0&&(list[i].recverCompare(name)==0)))count++;
            }
            delete [] checkOutcome;
            checkOutcome=new int[count];
            checkSize=count;
            count=0;
            for(i=0;i<size;i++){
                if((list[i].senderCompare(name)==0&&list[i].recverCompare(keyname)==0)||(name.compare(keyname)==0&&(list[i].recverCompare(name)==0))){
                    checkOutcome[count++]=i;
                }
            }
        }
        else if(usr.character==ADMIN_USER){
            int i,count;
            count=0;
            for(i=0;i<size;i++){
                if(list[i].recverCompare(keyname)==0)count++;
            }
            delete [] checkOutcome;
            checkOutcome=new int[count];
            checkSize=count;
            count=0;
            for(i=0;i<size;i++){
                if(list[i].recverCompare(keyname)==0){
                    checkOutcome[count++]=i;
                }
            }
        }
        else if(usr.character==PACKAGE_USER){
            int i,count;
            count=0;
            for(i=0;i<size;i++){
                if(list[i].recverCompare(keyname)==0&&list[i].pkgManCompare(usr.id)==0)count++;
            }
            delete [] checkOutcome;
            checkOutcome=new int[count];
            checkSize=count;
            count=0;
            for(i=0;i<size;i++){
                if(list[i].recverCompare(keyname)==0&&list[i].pkgManCompare(usr.id)==0){
                    checkOutcome[count++]=i;
                }
            }
        }
        return SUCCESS;
    }
    else return FAIL;
}
int packageList::searchSenderByRecver(string name, user& usr,string keyname) {
    if(keyname=="*"){
        this->checkSize=size;
        delete [] this->checkOutcome;
        checkOutcome=new int[size];
        int i;
        for(i=0;i<size;i++){
            checkOutcome[i]=i;
        }
        return SUCCESS;
    }
    if (usr.character == ADMIN_USER || (usr.character == NORMAL_USER && name.compare(usr.getName(usr)) == 0)||usr.character==PACKAGE_USER) {
        if((usr.character==NORMAL_USER&&name.compare(usr.getName(usr))==0)){
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if((list[i].recverCompare(name)==0&&list[i].senderCompare(keyname)==0)||(name.compare(keyname)==0&&(list[i].senderCompare(keyname)==0)))count++;
            }
            delete [] checkOutcome;
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if((list[i].recverCompare(name)==0&&list[i].senderCompare(keyname)==0)||(name.compare(keyname)==0&&(list[i].senderCompare(keyname)==0))){
                    checkOutcome[count++] = i;
                }
            }

        }
         else if(usr.character==ADMIN_USER){
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if(list[i].senderCompare(keyname)==0)count++;
            }
            delete [] checkOutcome;
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if(list[i].senderCompare(keyname)==0){
                    checkOutcome[count++] = i;
                }
            }
        }
        else if(usr.character==PACKAGE_USER){
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if(list[i].senderCompare(keyname)==0&&list[i].pkgManCompare(usr.id)==0)count++;
            }
            delete [] checkOutcome;
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if(list[i].senderCompare(keyname)==0&&list[i].pkgManCompare(usr.id)==0){
                    checkOutcome[count++] = i;
                }
            }
        }
        return SUCCESS;
    }
    else return FAIL;
}
int packageList::searchByRecver(string name, user& usr) {
    if(name=="*"){
        this->checkSize=size;
        delete [] this->checkOutcome;
        checkOutcome=new int[size];
        int i;
        for(i=0;i<size;i++){
            checkOutcome[i]=i;
        }
        return SUCCESS;
    }
    if (usr.character == ADMIN_USER || (usr.character == NORMAL_USER && name.compare(usr.getName(usr)) == 0)) {
        int i, count;
        count=0;
        for (i = 0; i < size; i++) {
            if (list[i].recverCompare(name) == 0)count++;
        }
        delete [] checkOutcome;
        checkOutcome = new int[count];
        checkSize = count;
        count = 0;
        for (i = 0; i < size; i++) {
            if (list[i].recverCompare(name) == 0) {
                checkOutcome[count++] = i;
            }
        }
        return SUCCESS;
    }
    else return FAIL;
}
int packageList::searchByTime(unsigned int down, unsigned int up, int type, user& usr) {
    if(down==0&&up==0){
        this->checkSize=size;
        delete [] this->checkOutcome;
        checkOutcome=new int[size];
        int i;
        for(i=0;i<size;i++){
            checkOutcome[i]=i;
        }
        return SUCCESS;
    }
    //type:1����ʱ�� 2����ʱ��
    if (type == 1) {
        if (usr.character == ADMIN_USER) {
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if (down <= list[i].sendTime &&list[i].sendTime <= up)count++;
            }
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if (down <= list[i].sendTime &&list[i].sendTime <= up) {
                    checkOutcome[count++] = i;
                }
            }
            return SUCCESS;
        }
        else if(usr.character==NORMAL_USER){
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if ((down <= list[i].sendTime &&list[i].sendTime <= up)&&(list[i].senderCompare(usr.getName(usr))==0|| list[i].recverCompare(usr.getName(usr)) == 0))count++;
            }
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if ((down <= list[i].sendTime &&list[i].sendTime <= up) && (list[i].senderCompare(usr.getName(usr)) == 0 || list[i].recverCompare(usr.getName(usr)) == 0)) {
                    checkOutcome[count++] = i;
                }
            }
            return SUCCESS;
        }
        else if(usr.character==PACKAGE_USER){
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if ((down <= list[i].sendTime &&list[i].sendTime <= up)&&(list[i].pkgManCompare(usr.id)==0))count++;
            }
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if ((down <= list[i].sendTime &&list[i].sendTime <= up) && (list[i].pkgManCompare(usr.id)==0)) {
                    checkOutcome[count++] = i;
                }
            }
            return SUCCESS;
        }
    }
    else if (type == 2) {
        if (usr.character == ADMIN_USER) {
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if (down <= list[i].recvTime &&list[i].recvTime <= up)count++;
            }
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if (down <= list[i].recvTime &&list[i].recvTime <= up) {
                    checkOutcome[count++] = i;
                }
            }
            return SUCCESS;
        }
        else if(usr.character==NORMAL_USER){
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if ((down <= list[i].recvTime &&list[i].recvTime <= up) && (list[i].senderCompare(usr.getName(usr)) == 0 || list[i].recverCompare(usr.getName(usr)) == 0))count++;
            }
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if ((down <= list[i].recvTime &&list[i].recvTime <= up) && (list[i].senderCompare(usr.getName(usr)) == 0 || list[i].recverCompare(usr.getName(usr)) == 0)) {
                    checkOutcome[count++] = i;
                }
            }
            return SUCCESS;
        }
        else if(usr.character==PACKAGE_USER){
            int i, count;
            count=0;
            for (i = 0; i < size; i++) {
                if ((down <= list[i].recvTime &&list[i].recvTime <= up) &&(list[i].pkgManCompare(usr.id)==0))count++;
            }
            checkOutcome = new int[count];
            checkSize = count;
            count = 0;
            for (i = 0; i < size; i++) {
                if ((down <= list[i].recvTime &&list[i].recvTime <= up) &&(list[i].pkgManCompare(usr.id)==0)) {
                    checkOutcome[count++] = i;
                }
            }
            return SUCCESS;
        }
    }
    return FAIL;
}
int packageList::searchByPackgeMan(string name, user &usr){
    if(name=="*"){
        this->checkSize=size;
        delete [] this->checkOutcome;
        checkOutcome=new int[size];
        int i;
        for(i=0;i<size;i++){
            checkOutcome[i]=i;
        }
        return SUCCESS;
    }
    if (usr.character == ADMIN_USER || (usr.character == PACKAGE_USER && name.compare(usr.getName(usr)) == 0))
    {
        int i, count;
        count=0;
        Users.searchByName(name,PACKAGE_USER);
        if(Users.getOutNum()==0){
            return FAIL;
        }
        int pkgManNum=Users.getOutcome()[0];
        for (i = 0; i < size; i++) {
            if (list[i].pkgManCompare(pkgManNum) == 0)count++;
        }
        delete [] checkOutcome;
        checkOutcome = new int[count];
        checkSize = count;
        count = 0;
        for (i = 0; i < size; i++) {
            if (list[i].pkgManCompare(pkgManNum) == 0) {
                checkOutcome[count++] = i;
            }
        }
        return SUCCESS;
    }
    else{
        return FAIL;
    }
}
int packageList::packageRecv(int pkgNum){
    this->list[pkgNum].recved();
}
int* packageList::getCheckOutcom(){
    return this->checkOutcome;
}
int packageList::getSize(){
    return this->size;
}
int packageList::setOutcome(int pkgNum){
    Packages.checkSize=1;
    delete [] Packages.checkOutcome;
    Packages.checkOutcome=new int;
    Packages.checkOutcome[0]=pkgNum;
    return 0;
}
int packageList::pkgManSendPkg(int pkgNum){
    this->list[pkgNum].status=PKG_UNRECV;
    return SUCCESS;
}
int packageList::search(int searchID, QString keyWord, int currentUser){
    return 0;
}
packageList::packageList(char* dict) {
    long long pkgNum_save;
    ifstream ifs;
    int pkgManSave;
    int status_save;
    string sender_save;
    string recvPhone_save;
    string sendAdress_save;
    string recvAdress_save;
    string recver_save;
    unsigned int sendTime_save;
    unsigned int recvTime_save;
    string itemDes_save;
    int itemType;
    int itemUnit;
    char common_save;
    int i;
    ifs.open(dict, ios::in);
    if (!ifs.is_open())
    {
        ofstream ofs;
        ofs.open(dict, ios::out);
        ofs.close();
    }
    int length;
    ifs >> length;
    if (ifs.fail()) {
        cout << "attention!" << endl;
    }
    else {
        size = length;
        list = new package[length];
        for (i = 0; i < length; i++) {
            if(ifs.fail()){
                cout << "attention!" << endl;
                break;
            }
            ifs >> pkgNum_save;
            ifs >> common_save;
            ifs >> status_save;
            ifs >> common_save;
            getline(ifs, sender_save, ',');
            getline(ifs, recvPhone_save, ',');
            getline(ifs, sendAdress_save, ',');
            getline(ifs, recvAdress_save, ',');
            getline(ifs, recver_save, ',');
            ifs >> sendTime_save;
            ifs >> common_save;
            ifs >> recvTime_save;
            ifs >> common_save;
            getline(ifs, itemDes_save,',');
            ifs >> pkgManSave;
            ifs >> common_save;
            ifs >> itemUnit;
            ifs >> common_save;
            ifs >> itemType;
            basicItem* itemSave=nullptr;
            switch (itemType) {
                case TYPE_BOOK:
                    itemSave=new bookItem;

                break;
                case TYPE_GLASS:
                    itemSave=new glassItem;
                break;
                case TYPE_NORMAL:
                    itemSave=new normalItem;
                break;

            }
            itemSave->setType(itemType);
            itemSave->unit=itemUnit;
            list[i] = package(pkgManSave,pkgNum_save, status_save, sender_save, recvPhone_save, sendAdress_save, recvAdress_save, recver_save, sendTime_save, recvTime_save, itemDes_save,itemSave);
        }
    }
    ifs.close();
}
int packageList::saveData(char* dict){
    ofstream ofs;
    ofs.open(dict, ios::out|ios::trunc);
    ofs<<size<<endl;
    int i;
    for (i = 0; i < size; i++) {
        list[i].printData(ofs);
    }
    return SUCCESS;
}

int userList::payMoney(int num, int fromUsr, int toUsr){
    this->list[fromUsr].money-=num;
}
user* userList::getuser(int id){
    if(id<size)return &list[id];
    else return nullptr;
}
int userList::getSize(){
    return size;
}
int userList::registerUser(string username, string name, string password, string phoneNumber,int type) {
    //����
    int i;

    cout<<"register!"<<endl;
    for (i = 0; i < size; i++) {
        if (list[i].checkRepeat(username) == 0) {

            return FAIL;
        }
    }
    user *newList = new user[size + 1];
    for (i = 0; i < size; i++) {
        newList[i] = list[i];
    }
    newList[i] = user(i, username, name, password, phoneNumber,type);
    cout<<"Register:"<<i<<username<<name<<password<<phoneNumber<<endl;
    delete []list;
    list=(user *)*(&newList);
    size++;
    return i;
}
int userList::getOutNum(){
    return this->outNum;
}
int* userList::getOutcome(){
    return this->output;
}
int userList::checkall(int type){
    int i,count;
    switch(type){
        case 0:
        count=0;
        for(i=0;i<size;i++){
            if(list[i].visible==VISABLE)count++;
        }
        outNum=count;

        delete []output;
        output=new int[count];
        count=0;
        for(i=0;i<size;i++){
            if(list[i].visible==VISABLE){
                output[count]=i;
                count++;
            }
        }
        return SUCCESS;
        case NORMAL_USER:
        case ADMIN_USER:
        case PACKAGE_USER:

            count=0;
            for(i=0;i<size;i++){
                if(list[i].character==type&&list[i].visible==VISABLE)count++;
            }
            outNum=count;

            delete []output;
            output=new int[count];
            count=0;
            for(i=0;i<size;i++){
                if(list[i].character==type&&list[i].visible==VISABLE){
                    output[count]=i;
                    count++;
                }
            }
            return SUCCESS;
    }
    return FAIL;

}
int userList::searchByName(string name,int type){
    int i,count;
    count=0;
    if(type==0){
        for(i=0;i<size;i++){
            if(list[i].compareName(name)==0&&list[i].visible==VISABLE)count++;
        }
        outNum=count;

        delete []output;
        output=new int[count];
        count=0;
        for(i=0;i<size;i++){
            if(list[i].compareName(name)==0&&list[i].visible==VISABLE){
                output[count]=i;
                count++;
            }
        }
    }
    else{
        for(i=0;i<size;i++){
            if(list[i].compareName(name)==0&&list[i].character==type&&list[i].visible==VISABLE)count++;
        }
        outNum=count;

        delete []output;
        output=new int[count];
        count=0;
        for(i=0;i<size;i++){
            if(list[i].compareName(name)==0&&list[i].character==type&&list[i].visible==VISABLE){
                output[count]=i;
                count++;
            }
        }
    }
    return count;


}
int userList::login(user temp) {//�ɹ�����ID�༴userList��Ӧλ�ã�ʧ�ܷ���Fail
    int i;
    int judge = 0;
    int save=0;
    for (i = 0; i < size; i++) {
        cout<<list[i].getName(list[i])<<endl;
        if (list[i].checkRepeat(temp) == 0&&list[i].visible==VISABLE) {
            list[i].state=USER_LOGGED;
            save=i;
            judge = 1;
            break;
        }
    }
    if (judge==1) {
        return save;
    }
    else return FAIL;
}
int userList::setCurrentNum(int num){
    currentNum=num;
    cout<<num<<"set!"<<endl;
    return num;
}
int userList::getCurrentNum(){
    return currentNum;
}
int userList::getAdmin(){
    int i;
    for(i=0;i<this->size;i++){
        if(this->list[i].character==ADMIN_USER)return i;
    }
    return FAIL;
}


int userList::userDelete(int num){
    if(num<this->getSize()&&this->list[num].character!=ADMIN_USER){
        list[num].visible=UNVISABLE;
        return SUCCESS;
    }
    else return FAIL;

}
int userList::updateUser(user usr){
    this->list[usr.id]=usr;
    return SUCCESS;
}
int userList::pkgManQinDing(){
    return this->pkgMan[rand()%this->pkgManSize];
}
int userList::saveData(char* dict) {
    ofstream ofs;
    ofs.open(dict, ios::trunc);
    ofs<<this->size<<endl;
    cout<<this->size<<endl;
    int i;
    for (i = 0; i < size; i++) {
        list[i].printData(ofs);
    }
    return SUCCESS;
}
userList::userList(char* dict) {
    ifstream ifs;
    int i, j;
    int id_save;
    int character_save;
    string username_str_save;
    string name_str_save;
    string address_save[DEFAULT_SIZE];
    int addressSize_save;
    string password_save;
    string phoneNumber_save;
    int money_save;
    char common_save;
    int visible_save;
    int state_save;
    ifs.open(dict, ios::in);
    if (!ifs.is_open())
    {
        ofstream ofs;
        ofs.open(dict, ios::out);
        ofs.close();
    }
    int length;
    ifs >> length;
    if (ifs.fail()) {
        cout << "usr数据库出现问题,请检查！" << endl;
    }
    else {
        size = length;
        delete [] list;
        list = new user[length];
        output=NULL;
        outNum=0;
        for (i = 0; i < length; i++) {
            if(ifs.fail()){
                cout << "usr数据库出现问题,请检查！" << endl;
                break;
            }
            ifs >> id_save;

            ifs >> common_save;

            ifs >> character_save;

            ifs >> common_save;
            getline(ifs, username_str_save, ',');
            getline(ifs, name_str_save, ',');
            ifs >> addressSize_save;
            ifs >> common_save;
            for (j = 0; j < addressSize_save; j++) {
                getline(ifs, address_save[j], ',');
            }
            getline(ifs, password_save, ',');
            getline(ifs, phoneNumber_save, ',');

            ifs >> money_save;
            ifs >> common_save;
            ifs >> state_save;
            ifs >> common_save;
            ifs >> visible_save;
            cout << phoneNumber_save<<endl;
            cout << id_save << ',' << character_save << ',' << username_str_save << ',' << name_str_save << ',' << addressSize_save << ','<<password_save<<endl;
            list[i] = user(i, character_save, username_str_save, name_str_save, addressSize_save, address_save, password_save, phoneNumber_save, money_save, state_save,visible_save);
        }
        pkgManInit();
    }

}
void userList::pkgManInit(){
    int i,count;
    count=0;
    for(i=0;i<this->size;i++){
        if(this->list[i].character==PACKAGE_USER){
            count++;
        }
    }
    this->pkgMan=new int[count];
    pkgManSize=count;
    count=0;
    for(i=0;i<this->size;i++){
        if(this->list[i].character==PACKAGE_USER){
            this->pkgMan[count++]=i;
        }
    }

}
void userList::userLogout(int id){
    Users.getuser(id)->state=USER_UNLOGIN;
    Users.setCurrentNum(-1);//退出用户登陆状态自动为-1
}
int glassItem::getPrice(){
    return this->unit*this->perPrice;
}
int bookItem::getPrice(){
    return this->unit*this->perPrice;
}
int normalItem::getPrice(){
    return this->unit*this->perPrice;
}

//��ַ����������Ӣ�Ķ���","
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

int basicItem::setType(int type){
   this->type=type;
    return SUCCESS;
}
void glassItem::printData(ofstream &ofs){
    ofs<<this->unit<<","<<this->type<<endl;
}
void normalItem::printData(ofstream &ofs){
    ofs<<this->unit<<","<<this->type<<endl;
}
void bookItem::printData(ofstream &ofs){
    ofs<<this->unit<<","<<this->type<<endl;
}
dataFragment::dataFragment(QString report){
    report=recvBuffer;
    QJsonDocument doc = QJsonDocument::fromJson(recvBuffer.toUtf8());
    if(doc.isNull())
        {
            qDebug()<< "String NULL"<< recvBuffer.toUtf8();
        }
    QJsonObject jsonObject = doc.object();
    this->paras=jsonObject.value("paras").toObject();
    this->funcNum=jsonObject.value("funcNum").toInt();
    this->classType=jsonObject.value("classType").toInt();
}
//路由函数：根据数据报的classType和funcNum字段判断将参数传入的函数和转换办法
QJsonObject socketManager::recvMesgProcess(dataFragment recvMsg){
    int needFeedback=NO;
    QJsonObject feedback;
    QJsonObject outcome;
    package savePkg;
    user saveUsr;
    int saveUsrNum;
    int *saveOutcome;
    int i;
    switch (recvMsg.classType) {
            case 1:
            switch (recvMsg.funcNum) {

                case 1:
                    feedback.insert("return",Packages.getPkg(recvMsg.paras.value("num").toInt()).toJsonObject());
                    needFeedback=YES;
                break;
                case 2:
                    feedback.insert("return",Packages.getCheckSize());
                    needFeedback=YES;
                break;
                case 3:
                    savePkg=package(recvMsg.paras.value("newpack").toObject());
                    feedback.insert("return",QString::number(Packages.add(savePkg)));
                    needFeedback=YES;
                break;
                case 4:
                    saveUsrNum=recvMsg.paras.value("id").toInt();
                    saveUsr=*Users.getuser(saveUsrNum);
                    Packages.searchRecverBySender(string(recvMsg.paras.value("name").toString().toLocal8Bit()),saveUsr,string(recvMsg.paras.value("keyWord").toString().toLocal8Bit()));
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                break;
                case 5:
                    saveUsrNum=recvMsg.paras.value("id").toInt();
                    saveUsr=*Users.getuser(saveUsrNum);
                    Packages.searchSenderByRecver(string(recvMsg.paras.value("name").toString().toLocal8Bit()),saveUsr,string(recvMsg.paras.value("keyWord").toString().toLocal8Bit()));
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                break;
                case 6:
                    outcome=recvMsg.paras.value("usr").toObject();
                    saveUsr=user(&outcome);
                    Packages.searchByRecver(string(recvMsg.paras.value("name").toString().toLocal8Bit()),saveUsr);
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                break;
                case 7:
                    outcome=recvMsg.paras.value("usr").toObject();
                    saveUsr=user(&outcome);
                    Packages.searchByTime(recvMsg.paras.value("down").toString().toUInt(),recvMsg.paras.value("up").toString().toUInt(),recvMsg.paras.value("type").toInt(),saveUsr);
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                break;
                case 8:
                    outcome=recvMsg.paras.value("usr").toObject();
                    saveUsr=user(&outcome);
                    Packages.searchByPackgeMan(string(recvMsg.paras.value("name").toString().toLocal8Bit()),saveUsr);
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                break;
                case 9:
                    feedback.insert("return",Packages.packageRecv(recvMsg.paras.value("pkgNum").toInt()));
                    needFeedback=YES;
                break;
                case 10:
                    saveOutcome=Packages.getCheckOutcom();

                    for(i=0;i<Packages.getCheckSize();i++){
                       outcome.insert(QString::number(i),QString::number(saveOutcome[i]));
                    }
                    feedback.insert("return",outcome);
                    needFeedback=YES;
                break;
                case 11:
                    feedback.insert("return",Packages.getSize());
                    needFeedback=YES;
                break;
                case 12:
                    Packages.setOutcome(recvMsg.paras.value("pkgNum").toInt());
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                case 13:
                    Packages.pkgManSendPkg(recvMsg.paras.value("pkgNum").toInt());
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                break;
                default:
                    break;

            }
        break;
        case 2:
            switch (recvMsg.funcNum) {
               case 1:
                Users.payMoney(recvMsg.paras.value("num").toInt(),recvMsg.paras.value("fromUsr").toInt(),recvMsg.paras.value("toUsr").toInt());
                feedback.insert("return",SUCCESS);
                needFeedback=YES;
                break;
               case 2:
                feedback.insert("return",Users.getuser(recvMsg.paras.value("id").toInt())->toJsonObject());
                needFeedback=YES;
                break;
               case 3:
                feedback.insert("return",Users.getSize());
                needFeedback=YES;
                break;
               case 4:
                feedback.insert("return",Users.registerUser(
                                    string(recvMsg.paras.value("username").toString().toLocal8Bit()),
                                    string(recvMsg.paras.value("name").toString().toLocal8Bit()),
                                    string(recvMsg.paras.value("password").toString().toLocal8Bit()),
                                    string(recvMsg.paras.value("phoneNumber").toString().toLocal8Bit()),
                                    recvMsg.paras.value("type").toInt()
                                    ));

                needFeedback=YES;
                break;
               case 5:
                feedback.insert("return",Users.getOutNum());
                needFeedback=YES;
                break;
               case 6:
                saveOutcome=Users.getOutcome();

                for(i=0;i<Users.getOutNum();i++){
                   outcome.insert(QString::number(i),QString::number(saveOutcome[i]));
                }
                feedback.insert("return",outcome);
                needFeedback=YES;
                break;
               case 7:
                feedback.insert("return",Users.checkall(recvMsg.paras.value("type").toInt()));
                needFeedback=YES;
                break;
               case 8:
                feedback.insert("return",Users.searchByName(string(recvMsg.paras.value("name").toString().toLocal8Bit()),
                                   recvMsg.paras.value("type").toInt()));

                needFeedback=YES;
                break;
            case 9:
                outcome=recvMsg.paras.value("temp").toObject();
                saveUsr=user(&outcome);
                feedback.insert("return",Users.login(saveUsr));
                needFeedback=YES;
                break;
            case 10:
                Users.setCurrentNum(recvMsg.paras.value("num").toInt());
                feedback.insert("return",SUCCESS);
                needFeedback=YES;
                break;
            case 11:
                feedback.insert("return",Users.getCurrentNum());
                needFeedback=YES;
                break;
            case 12:
                feedback.insert("return",Users.getAdmin());
                needFeedback=YES;
                break;
            case 13:
                feedback.insert("return",Users.userDelete(recvMsg.paras.value("num").toInt()));
                needFeedback=YES;
                break;
            case 14:
                feedback.insert("return",Users.pkgManQinDing());
                needFeedback=YES;
                break;
            }
        break;
        case 3:
            switch (recvMsg.funcNum) {
                case 1:
                    outcome=recvMsg.paras.value("usr").toObject();
                    saveUsr=user(&outcome);
                    Users.updateUser(saveUsr);
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                break;
                case 2:
                    Users.userLogout(recvMsg.paras.value("id").toString().toInt());
                    feedback.insert("return",SUCCESS);
                    needFeedback=YES;
                break;
            }

        break;

    }
    if(needFeedback==YES){
        feedback.insert("classType",recvMsg.classType);
        feedback.insert("funcNum",recvMsg.funcNum);
        feedback.insert("need",YES);

    }
    return feedback;

}
socketManager::socketManager(int id){
    this->id=id;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerFace w;
    w.show();

    return a.exec();
}
