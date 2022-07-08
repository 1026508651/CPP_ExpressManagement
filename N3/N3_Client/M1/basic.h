#include<iostream>
#include<string>
#include <fstream>
#include<QJsonObject>
#include<time.h>
#include <QString>
#include "csocket.h"
//socket沟通数据报结构
//|int classType|int funcNum|QJsonObject paras|
//|调用函数类名|调用函数编号|调用函数参数|
//构造函数等基础函数调用保持不变，涉及数据库操作和数据修改的函数纳入编号范围
//数据全部私有封装 所有修改数据全部通过函数进行
#define DEFAULT_SIZE 50 //默认大小
#define PACK_FEE 15 //需求1的快递费
#define USER_UNLOGIN 0 //用户状态设置：未登录
#define USER_LOGGED 1 //用户状态设置：已登录
#define NORMAL_USER 1 // 用户类别:普通用户
#define ADMIN_USER 2 //用户类别：管理员用户
#define PACKAGE_USER 3//用户类别: 快递员
#define PKG_INIT 0 //快递类别：初始化快递（未填写 未寄送）
#define PKG_WAITSEND 1 //快递类别 待揽收
#define PKG_UNRECV 2 //快递类别：已寄送未收到
#define PKG_RECV 3 //快递类别：已寄送已收到
#define FAIL -1//通用返回值 失败
#define SUCCESS 0//通用返回值 成功
#define TYPE_GLASS 1//物品类别：易碎品
#define TYPE_BOOK 2//物品类别：书本
#define TYPE_NORMAL 3//物品类别：一般物品
#define VISABLE 0//删除用户但不删除快递的解决方案：设置可视参数，用户不可视则不可登录或调用任何接口，但其数据保留且可供读取
#define UNVISABLE 1//
#define SEARCH_BY_PKGMAN 5//搜索类别
//Time 2204301159->unsigned int????????
//tips1:数据库不能为空 中间数据不要直接输出到cout中 否则会显示乱码
using namespace std;

class basicItem{
    public:
        virtual int getPrice()=0;
        int setType(int type);
        int unit;
        int type;
};
class glassItem:public basicItem{
    public:
        int getPrice();
    private:
        const int perPrice=8;
};
class bookItem:public basicItem{
    public:
        int getPrice();
    private:
        const int perPrice=2;
};
class normalItem:public basicItem{
    public:
        int getPrice();
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
    package(QJsonObject paras);
    int status;
    long long pkgNum;
    unsigned int sendTime;
    unsigned int recvTime;
    package();
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
    int updateUser(const user usr);
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
    user(QJsonObject* paras);
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
    user getuser(int id);
    void pkgManInit();
    int pkgManQinDing();
    int userDelete(int num);
    int *getOutcome();
    int search(int searchID,QString keyWord,int currentUser);
    int userLogout(int id);
};
class packageList {
   private:

    int size;
    int *checkOutcome;
    int checkSize;
    package *list;
public:

    int saveData(char* dict);


    int searchByPackgeMan(string name,user&usr);
    int searchByRecver(string name,user& usr);
    int searchRecverBySender(string name,user& usr,string keyname);
    int searchSenderByRecver(string name,user& usr,string keyname);
    int searchByTime(unsigned int down, unsigned int up, int type,user& usr);
    int setOutcome(int pkgNum);
    int add(package& newpack);//
    int getSize();
    int getCheckSize();
    package getPkg(int num);
    int packageRecv(int pkgNum);
    int* getCheckOutcom();
    int search(int searchID,QString keyWord,int currentUser);
    int pkgManSendPkg(int pkgNum);
};

string timeconvert(unsigned int time);



