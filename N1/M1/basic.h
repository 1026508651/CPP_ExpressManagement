#include<iostream>
#include<string>
#include <fstream>
#define DEFAULT_SIZE 50 //默认大小
#define PACK_FEE 15 //需求1的快递费
#define USER_UNLOGIN 0 //用户状态设置：未登录
#define USER_LOGGED 1 //用户状态设置：已登录
#define NORMAL_USER 1 // 用户类别:普通用户
#define ADMIN_USER 2 //用户类别：管理员用户
#define PKG_INIT 0 //快递类别：初始化快递（未填写 未寄送）
#define PKG_UNRECV 1 //快递类别：已寄送未收到
#define PKG_RECV 2 //快递类别：已寄送已收到
#define FAIL -1//通用返回值 失败
#define SUCCESS 0//通用返回值 成功
#include<time.h> 

//tips1:数据库不能为空 中间数据不要直接输出到cout中 否则会显示乱码
using namespace std;
class package {
private:

	string sender;
    string recvPhone;
	string sendAdress;
	string recvAdress;
	string recver;
	string itemDes;
public:
    int status;
    long long pkgNum;
	package();
    package(long long pkgNums);
    package(long long pkgNum_save, int status_save, string sender_save, string recvPhone_save, string sendAdress_save, string recvAdress_save, string recver_save, unsigned int sendTime_save, unsigned int recvTime_save, string itemDes_save);
    int packWrite(const string& recvName,string recvPhones,const string& recvAddress,const string& itemDess);//????
	int packWrite(const string& senders, const string& sendAdress);
	int recved();
    string getStatus();
    string getSender();
    string getRecver();
    string getRecvPhone();
    string getSendAdress();
    string getRecvAdress();
	int printData(ofstream& ofs);
	unsigned int sendTime;
	unsigned int recvTime;
	int senderCompare(string someone);
	int recverCompare(string someone);
};

class user {
private:
	int id;
	string username;
	string name;
	string password;
	int money;


public:
    int addressSize;
    string phoneNumber;
    string address[DEFAULT_SIZE];
    int state;//????
	int character;
    string getUsername();
    string getCharacter();

	user();
    user(int ids, int characters, string usernames, string names, int adressSizes, string adress[], string passwords, string phoneNumbers, int moneys, int states);
    user(int ids, string username, string name, string password, string phoneNumber);
	int getMoney();
    int checkPasswd(string temp);
    int changePasswd(string temp);
    int payMoney(int num);
	string getName();
	int addMoney(int num);
    long long sendPkg(long long pkgNum/*分配单号*/, int num/*选择第几个地址，默认为0*/,string recvName,int recvNum,string recvPhone,string itemDes);
	int recvPkg(package pack);
	int addAdress(string newAdress);
	int checkRepeat(string newUsername);
	int checkRepeat(user temp);
	int compareName(string otherName);
	int printData(ofstream& ofs);

};
//用户数据库和包裹数据库均有一个搜索缓冲区及其大小，用于存储搜索结果的索引
class userList {
private:


    int size;

    int currentNum;
public:
    user* list;
    int *output;
    int outNum;
	userList(char* dict);
    int getAdmin();

    int getCurrentNum();
    int setCurrentNum(int num);
    int registerUser(string username, string name, string password, string phoneNumber);//?????????ID
	int saveData(char* dict);
    int getSize();
    int checkall();
	int searchByName(string name);//return ????/*??*/
    int login(user temp);//????????????Userlist???
    user* getuser(int id);
};
class packageList {
public:
    package *list;
    int size;
    int *checkOutcome;
    int checkSize;
    int saveData(char* dict);

    packageList(char* dict);//
    int searchByRecver(string name,user& usr);
    int searchRecverBySender(string name,user& usr,string keyname);
    int searchSenderByRecver(string name,user& usr,string keyname);
    int searchByTime(unsigned int down, unsigned int up, int type,user& usr);
    int add(package& newpack);//
};
string timeconvert(unsigned int time);



