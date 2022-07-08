#include<iostream>
#include<string>
#include <fstream>
#define DEFAULT_SIZE 50 //Ĭ�ϴ�С
#define PACK_FEE 15 //����1�Ŀ�ݷ�
#define USER_UNLOGIN 0 //�û�״̬���ã�δ��¼
#define USER_LOGGED 1 //�û�״̬���ã��ѵ�¼
#define NORMAL_USER 1 // �û����:��ͨ�û�
#define ADMIN_USER 2 //�û���𣺹���Ա�û�
#define PKG_INIT 0 //�����𣺳�ʼ����ݣ�δ��д δ���ͣ�
#define PKG_UNRECV 1 //�������Ѽ���δ�յ�
#define PKG_RECV 2 //�������Ѽ������յ�
#define FAIL -1//ͨ�÷���ֵ ʧ��
#define SUCCESS 0//ͨ�÷���ֵ �ɹ�
#include<time.h> 

//tips1:���ݿⲻ��Ϊ�� �м����ݲ�Ҫֱ�������cout�� �������ʾ����
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
    long long sendPkg(long long pkgNum/*���䵥��*/, int num/*ѡ��ڼ�����ַ��Ĭ��Ϊ0*/,string recvName,int recvNum,string recvPhone,string itemDes);
	int recvPkg(package pack);
	int addAdress(string newAdress);
	int checkRepeat(string newUsername);
	int checkRepeat(user temp);
	int compareName(string otherName);
	int printData(ofstream& ofs);

};
//�û����ݿ�Ͱ������ݿ����һ�����������������С�����ڴ洢�������������
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



