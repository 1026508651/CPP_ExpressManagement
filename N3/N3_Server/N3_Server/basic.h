
#include<iostream>
#include<string>
#include <fstream>
#include <QJsonObject>
#include <QJsonDocument>
#define DEFAULT_SIZE 50
#define PACK_FEE 15
#define USER_UNLOGIN 0
#define NORMAL_USER 1
#define ADMIN_USER 2
#define PACKAGE_USER 3
#define PKG_INIT 0
#define PKG_WAITSEND 1
#define PKG_UNRECV 2
#define PKG_RECV 3
#define USER_LOGGED 1
#define FAIL -1
#define SUCCESS 0
#include<time.h>
#define TYPE_GLASS 1
#define TYPE_BOOK 2
#define TYPE_NORMAL 3
#define VISABLE 0
#define UNVISABLE 1
#include <QString>
#define NO 0
#define YES 1
#define SEARCH_BY_PKGMAN 5
//Time 2204301159->unsigned int????????
//tips1:数据库不能为空 中间数据不要直接输出到cout中 否则会显示乱码
using namespace std;
class basicItem{
    public:
        virtual int getPrice()=0;
        virtual void printData(ofstream& ofs)=0;
        int setType(int type);
        int unit;
        int type;



};
class glassItem:public basicItem{
    public:
        int getPrice();
        void printData(ofstream& ofs);

    private:
        const int perPrice=8;

};
class bookItem:public basicItem{
    public:
        int getPrice();
        void printData(ofstream& ofs);
    private:
        const int perPrice=2;
};
class normalItem:public basicItem{
    public:
        int getPrice();
        void printData(ofstream& ofs);
    private:
        const int perPrice=5;
};
class package {
private:
    int packageMan;
    string sender;
    string recvPhone;
    string sendAdress;
    string recvAdress;
    string recver;
    string itemDes;
    basicItem* item;

public:
    int status;
    long long pkgNum;
    unsigned int sendTime;
    unsigned int recvTime;
    package();
    package(QJsonObject paras);
    package(long long pkgNums,int type,int unit);
    package(int packageMan,long long pkgNum_save, int status_save, string sender_save, string recvPhone_save, string sendAdress_save, string recvAdress_save, string recver_save, unsigned int sendTime_save, unsigned int recvTime_save, string itemDes_save,basicItem* itemSave);
    int packWrite(const string& recvName,string recvPhones,const string& recvAddress,const string& itemDess);//????
    int packWrite(const string& senders, const string& sendAdress);
    int recved();
    int sendPrint();
    int recvedPrint();
    string getStatus();
    string getSender();
    string getRecver();
    string getRecvPhone();
    string getSendAdress();
    string getRecvAdress();
    int printData(ofstream& ofs);
    int senderCompare(string someone);
    int recverCompare(string someone);
    int pkgManCompare(int man);
    int getPkgMan();
    int getPrice();
    QJsonObject toJsonObject();
};

class user {
private:

    string username;
    string name;
    //????????
    string password;



public:
    int money;
    int id;
    int addressSize;
    int visible;
    string phoneNumber;
    string address[DEFAULT_SIZE];
    int state;//????
    int character;
    string getUsername();
    string getCharacter();

    user();
    user(QJsonObject *usr);
    user(int ids, int characters, string usernames, string names, int adressSizes, string adress[], string passwords, string phoneNumbers, int moneys, int states,int visible);
    user(int ids, string username, string name, string password, string phoneNumber,int type=NORMAL_USER);
    int getMoney();
    int checkPasswd(string temp);
    int changePasswd(string temp);

    string getName(user& usr);
    int addMoney(int num);
    long long sendPkg(long long pkgNum/*分配单号*/, int num/*选择第几个地址，默认为0*/,string recvName,int recvNum,string recvPhone,string itemDes,int type,int unit);
    int recvPkg(int pkgNum);
    int addAdress(string newAdress);
    int checkRepeat(string newUsername);
    int checkRepeat(user temp);
    int compareName(string otherName);
    int printData(ofstream& ofs);
    int packgeUsrSendPkg(int i);
    QJsonObject toJsonObject();
};
class userList {
private:

    int pkgManSize;
    int size;
    int *pkgMan;
    int currentNum;

    int *output;
    int outNum;
    user* list;
public:

    userList(char* dict);
    int getAdmin();
    int payMoney(int num,int fromUsr,int toUsr);
    int getCurrentNum();
    int setCurrentNum(int num);
    int getOutNum();
    int registerUser(string username, string name, string password, string phoneNumber,int type=NORMAL_USER);
    int saveData(char* dict);
    int getSize();
    int checkall(int type);
    int searchByName(string name,int type);//return ????/*??*/
    int login(user temp);//????????????Userlist???
    user* getuser(int id);
    void pkgManInit();
    int pkgManQinDing();
    int userDelete(int num);
    int *getOutcome();
    int search(int searchID,QString keyWord,int currentUser);
    int updateUser(user usr);
    void userLogout(int id);
};
class packageList {
   private:

    int size;
    int *checkOutcome;
    int checkSize;
    package *list;
public:

    int saveData(char* dict);
    packageList(char* dict);//
    int searchByPackgeMan(string name,user&usr);
    int searchByRecver(string name,user& usr);
    int searchRecverBySender(string name,user& usr,string keyname);
    int searchSenderByRecver(string name,user& usr,string keyname);
    int searchByTime(unsigned int down, unsigned int up, int type,user& usr);
    int add(package& newpack);//
    int getSize();
    int getCheckSize();
    package getPkg(int num);
    int packageRecv(int pkgNum);
    int* getCheckOutcom();
    int search(int searchID,QString keyWord,int currentUser);
    int pkgManSendPkg(int pkgNum);
    int setOutcome(int pkgNum);
};
string timeconvert(unsigned int time);

class dataFragment{//数据报类
public:
    int classType;
    int funcNum;
    QJsonObject paras;
    dataFragment(QString report);
    dataFragment();

};
class socketManager{//websocket处理manager

    public:

    int id;
    QJsonObject recvMesgProcess(dataFragment recvMsg);
    socketManager(int id);//便于实现多线程


};

