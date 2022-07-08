#include<iostream>
#include<string>
#include<time.h>
#include <fstream>
#define DEFAULT_SIZE 50 //Ĭ�ϴ�С
#define USER_UNLOGIN 0 //�û�״̬���ã�δ��¼
#define USER_LOGGED 1 //�û�״̬���ã��ѵ�¼
#define NORMAL_USER 1 // �û����:��ͨ�û�
#define ADMIN_USER 2 //�û���𣺹���Ա�û�
#define PACKAGE_USER 3//�û����: ���Ա
#define PKG_INIT 0 //�����𣺳�ʼ����ݣ�δ��д δ���ͣ�
#define PKG_WAITSEND 1 //������ ������
#define PKG_UNRECV 2 //�������Ѽ���δ�յ�
#define PKG_RECV 3 //�������Ѽ������յ�
#define FAIL -1//ͨ�÷���ֵ ʧ��
#define SUCCESS 0//ͨ�÷���ֵ �ɹ�
#define TYPE_GLASS 1//��Ʒ�������Ʒ
#define TYPE_BOOK 2//��Ʒ����鱾
#define TYPE_NORMAL 3//��Ʒ���һ����Ʒ
#define VISABLE 0//ɾ���û�����ɾ����ݵĽ�����������ÿ��Ӳ������û��������򲻿ɵ�¼������κνӿڣ��������ݱ����ҿɹ���ȡ
#define UNVISABLE 1//
//Time 2204301159->unsigned int???????? 
//tips1:���ݿⲻ��Ϊ�� �м����ݲ�Ҫֱ�������cout�� �������ʾ����
using namespace std;
class basicItem{//������Ʒ��
    public:
        virtual int getPrice()=0;//���麯��
        virtual void printData(ofstream& ofs)=0;//��������
        int setType(int type);//������
        int unit;
        int type;



};
class glassItem:public basicItem{//������Ʒ��
    public:
    //�������麯����ʵ��
        int getPrice();
        void printData(ofstream& ofs);

    private:
        const int perPrice=8;

};
class bookItem:public basicItem{//�鱾��Ʒ��
    public:
    //�������麯����ʵ��
        int getPrice();
        void printData(ofstream& ofs);
    private:
        const int perPrice=2;
};
class normalItem:public basicItem{//һ����Ʒ��
    public:
    //�������麯����ʵ��
        int getPrice();
        void printData(ofstream& ofs);
    private:
        const int perPrice=5;
};
class package {
    //��������Ʒ���Ĺ�ϵ����Ʒװ�ڰ����У�������ָ����ʽ�洢��Ʒ��
    //����ָ�����ָ��������
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
    package(long long pkgNums,int type,int unit);
    package(int packageMan,long long pkgNum_save, int status_save, string sender_save, string recvPhone_save, string sendAdress_save, string recvAdress_save, string recver_save, unsigned int sendTime_save, unsigned int recvTime_save, string itemDes_save,basicItem* itemSave);
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
	int senderCompare(string someone);
	int recverCompare(string someone);
    int pkgManCompare(int man);
    int getPkgMan();
    int getPrice();
};

class user {
private:

	string username;
	string name;
    //????????
	string password;

	int money;


public:
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
    user(int ids, int characters, string usernames, string names, int adressSizes, string adress[], string passwords, string phoneNumbers, int moneys, int states,int visible);
    user(int ids, string username, string name, string password, string phoneNumber,int type=NORMAL_USER);
	int getMoney();
    int checkPasswd(string temp);
    int changePasswd(string temp);
    int payMoney(int num,int paywho);
    string getName(user& usr);
	int addMoney(int num);
    long long sendPkg(long long pkgNum/*���䵥��*/, int num/*ѡ��ڼ�����ַ��Ĭ��Ϊ0*/,string recvName,int recvNum,string recvPhone,string itemDes,int type,int unit);
	int recvPkg(package pack);
	int addAdress(string newAdress);
	int checkRepeat(string newUsername);
	int checkRepeat(user temp);
	int compareName(string otherName);
	int printData(ofstream& ofs);
    int packgeUsrSendPkg(package &pack);
};
class userList {
private:

    int pkgManSize;
    int size;
    int *pkgMan;//���п��Ա����������
    int currentNum;
public:
    user* list;
    int *output;
    int outNum;
	userList(char* dict);
    int getAdmin();

    int getCurrentNum();
    int setCurrentNum(int num);

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

};
class packageList {
public:
    package *list;
    int size;
    int *checkOutcome;
    int checkSize;
    int saveData(char* dict);

    packageList(char* dict);//
    int checkALL(user usr);
    int searchByPackgeMan(string name,user&usr);
    int searchByRecver(string name,user& usr);
    int searchRecverBySender(string name,user& usr,string keyname);
    int searchSenderByRecver(string name,user& usr,string keyname);
    int searchByTime(unsigned int down, unsigned int up, int type,user& usr);
    int add(package& newpack);//
};
string timeconvert(unsigned int time);//���ߺ��� û��Ҫ�ٵ���дһ������



