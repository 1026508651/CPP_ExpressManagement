#include "variables.h"
#include "login.h"
#include <QTextCodec>
#include "sstream"
#include <QApplication>
static unsigned int Time = 0;
char userLocal[DEFAULT_SIZE] = "users.txt";//�û���Ϣ�洢�ļ�
char packageLocal[DEFAULT_SIZE] = "packages.txt";//�����Ϣ�洢�ļ�
userList Users = userList(userLocal);//�������ʼ���û�����
packageList Packages = packageList(packageLocal);//�������ʼ���������
/*
 * Users��Packages��Ϊexternȫ�ֱ�������Ҫ����Main.cpp�Ŀ���Ǳ�֤��ķ�װ������
*/

//��������
package::package(long long pkgNums){
    //���Ϳ��ʱ������ݵ��ŵİ������캯��
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
}
package::package() {
    //Ĭ�ϰ������캯��
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
package::package(long long pkgNum_save, int status_save, string sender_save, string recvPhone_save, string sendAdress_save, string recvAdress_save, string recver_save, unsigned int sendTime_save, unsigned int recvTime_save, string itemDes_save) {
    //�ļ���ȡ��Ϣ�����������ݿ�Ĺ��캯��
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
}
int package::recved() {
    //�հ����������޸�״̬�������հ���ʱ��
	status = PKG_RECV;
	time_t t;
	struct tm *tmp;
	time(&t);
	tmp = localtime(&t);
	//2204301710
	Time = tmp->tm_year % 100 * 100000000 + (tmp->tm_mon + 1) * 1000000 + (tmp->tm_mday) * 10000 + tmp->tm_hour * 100 + tmp->tm_min;
    //��ݷ�ȫ������ ����ȡ��
    //�տ�ݰ����� ��һ��Ŀ�ݷ�ת�����Ա
    recvTime = Time;
    return SUCCESS;
}
int package::senderCompare(string someone){
    //Ϊ��֤��װ�����������ռ��˶Ա�
	return sender.compare(someone);
}
int package::recverCompare(string someone) {
    //Ϊ��֤��װ���������ͷ����˶Ա�
	return recver.compare(someone);
}
int package::packWrite(const string& recvName,string recvPhones,const string& recvAddress,const string& itemDess){
    //�ļ�ʱ������д��ݵ�
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
int package::packWrite(const string& senders,const string& sendAddress){
    //��ݹ�˾д��ݵ�
	sender=senders;
	sendAdress=sendAddress;
	status=PKG_UNRECV;
	return status;
}
int package::printData(ofstream& ofs){
    //�����ļ����ļ���
	ofs<<pkgNum<<','<<status<<','<<sender<<','<<recvPhone<<','<<sendAdress<<','<<recvAdress<<','<<recver<<','<<sendTime<<','<<recvTime<<','<<itemDes<<endl;
    return SUCCESS;
}
string package::getStatus(){
    //�������״̬Ϊ�ַ�����Ŀ���Ǳ��ַ�װ
    string s;
    switch(this->status){
        case PKG_INIT:
        s=string("��д��");
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
//����Ϊ���ƻ���װ��������Ϣ��ȡ����
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
//�������ݿⲿ��
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
//�����������������ƥ�䣩
int packageList::searchRecverBySender(string name,user& usr,string keyname){
    //�����û�Ȩ�ޣ�����Ա/��ͨ�û������ظ�������ص�������� ����������
	if(usr.character==ADMIN_USER||(usr.character==NORMAL_USER&&name.compare(usr.getName())==0)){
        if((usr.character==NORMAL_USER&&name.compare(usr.getName())==0)){
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
        else{
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
		return SUCCESS;
	}
	else return FAIL;
}
int packageList::searchSenderByRecver(string name, user& usr,string keyname) {
    //�����û�Ȩ�ޣ�����Ա/��ͨ�û������ظ�������ص�������� ����������
	if (usr.character == ADMIN_USER || (usr.character == NORMAL_USER && name.compare(usr.getName()) == 0)) {
        if((usr.character==NORMAL_USER&&name.compare(usr.getName())==0)){
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
        else{
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
        return SUCCESS;
	}
	else return FAIL;
}
int packageList::searchByRecver(string name, user& usr) {
    //�����տ��ҳ���ֱ������
    if (usr.character == ADMIN_USER || (usr.character == NORMAL_USER && name.compare(usr.getName()) == 0)) {
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
    //ͨ��ʱ��������֧������������ʵ�����ý�������ʱ��㣨�����ʽʶ���Ϊ���ӣ�
    //��֧��type��ʾ�����ĳ�ʱ��/����ʱ��
    //������֧Ϊ��Ȩ������Ա/��ͨ�û���
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
		else {
			int i, count;
            count=0;
			for (i = 0; i < size; i++) {
				if ((down <= list[i].sendTime &&list[i].sendTime <= up)&&(list[i].senderCompare(usr.getName())==0|| list[i].recverCompare(usr.getName()) == 0))count++;
			}
			checkOutcome = new int[count];
			checkSize = count;
			count = 0;
			for (i = 0; i < size; i++) {
				if ((down <= list[i].sendTime &&list[i].sendTime <= up) && (list[i].senderCompare(usr.getName()) == 0 || list[i].recverCompare(usr.getName()) == 0)) {
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
		else {
			int i, count;
            count=0;
			for (i = 0; i < size; i++) {
				if ((down <= list[i].recvTime &&list[i].recvTime <= up) && (list[i].senderCompare(usr.getName()) == 0 || list[i].recverCompare(usr.getName()) == 0))count++;
			}
			checkOutcome = new int[count];
			checkSize = count;
			count = 0;
			for (i = 0; i < size; i++) {
				if ((down <= list[i].recvTime &&list[i].recvTime <= up) && (list[i].senderCompare(usr.getName()) == 0 || list[i].recverCompare(usr.getName()) == 0)) {
					checkOutcome[count++] = i;
				}
			}
			return SUCCESS;
		}
	}
    return FAIL;
}
packageList::packageList(char* dict) {
    //���캯����ͨ�������ļ�Ŀ¼ֱ�ӹ������ݿ�
    long long pkgNum_save;
	ifstream ifs;
	int status_save;
	string sender_save;
    string recvPhone_save;
	string sendAdress_save;
	string recvAdress_save;
	string recver_save;
	unsigned int sendTime_save;
	unsigned int recvTime_save;
	string itemDes_save;
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
		cout << "pkg���ݿ��쳣�������飡" << endl;
	}
	else {
		size = length;
		list = new package[length];
		for (i = 0; i < length; i++) {
            if(ifs.fail()){
                cout << "pkg���ݿ��쳣�������飡" << endl;
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
			getline(ifs, itemDes_save);
			list[i] = package(pkgNum_save, status_save, sender_save, recvPhone_save, sendAdress_save, recvAdress_save, recver_save, sendTime_save, recvTime_save, itemDes_save);
		}
	}
	ifs.close();
}
int packageList::saveData(char* dict){
    //�������� ���ݳ־û�
	ofstream ofs;
	ofs.open(dict, ios::out|ios::trunc);
	ofs<<size<<endl;
	int i;
	for (i = 0; i < size; i++) {
		list[i].printData(ofs);
	}
	return SUCCESS;
}
//�û�����
int user::addAdress(string newAdress) {
    //�û����ӵ�ַ
	if (addressSize < DEFAULT_SIZE) {
		address[addressSize] = newAdress;
		addressSize++;
        return SUCCESS;
	}
	else return FAIL;
}
int user::checkPasswd(string temp){
    //�û��޸����� ��Ҫһ���������ܵ��ô˺��� ��˺���ֻʵ���޸����뺯��
    return temp.compare(this->password);
}
string user::getName(){
    //��ȡ�û��������ƻ���װ�� ��ֻ�е�¼�û�����ʹ��
	if(state==USER_LOGGED)return name;
    else return "";
}
long long user::sendPkg(long long pkgNum/*���䵥��*/, int num/*ѡ��ڼ�����ַ��Ĭ��Ϊ0*/,string recvName,int recvNum,string recvPhone,string itemDes) {
    //���Ϳ�� ��������Ϣ���й鵵
    if (state == USER_LOGGED&&this->money>=PACK_FEE) {
		//��ȡ������Ϣ
		package pkg = package(pkgNum);
        pkg.packWrite(recvName, recvPhone, address[recvNum], itemDes);
		pkg.packWrite(name, address[num]);
		Packages.add(pkg);
        this->payMoney(PACK_FEE);
        Users.getuser(Users.getAdmin())->addMoney(PACK_FEE);
		return pkgNum;
	}
	else return FAIL;
}
string user::getUsername(){
    //���ƻ���װ�Ļ�ȡ��Ϣ����
    return this->username;
}
user::user() {
    //�û���Ĭ�Ϲ��캯������������δ��ȡ�û���Ϣʱ����ʱ���죩
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
user::user(int ids, int characters, string usernames, string names, int adressSizes, string adress[], string passwords, string phoneNumbers, int moneys, int states) {
    //��ȡ�ļ����ݿ���û����캯��
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
}
user::user(int ids, string usernames, string names, string passwords, string phoneNumbers) {
    //�û�ע��ʱ�ĺ�̨�û������ʼ������
	id = ids;
	username = usernames;
	name = names;
	password = passwords;
	phoneNumber = phoneNumbers;
	character = NORMAL_USER;
	addressSize = 0;
	money = 0;
    state = USER_UNLOGIN;
}
string user::getCharacter(){
    //���ư�����getStatus����
    switch(this->character){
        case NORMAL_USER:
            return string("�û�");
        case ADMIN_USER:
            return string("����Ա");
    }
    return "";
}
int user::getMoney() {
    //���ƻ���װ ��ֹ��Money����ֱ���޸�
	return money;
}
int user::addMoney(int num) {
    //֧�ָ��� �ı�Ŀǰ�˻�Ǯ��
	money += num;
	return money;
}
int user::payMoney(int num){
    //�ú�������ֱ�� ��Ǯ
    money -= num;
    return money;
}
int user::recvPkg(package pack) {
    //�տ�� �����û��Ϳ��֮��Ĺ�ϵ
	Packages.list[pack.pkgNum].recved();
    return SUCCESS;
}
int user::checkRepeat(string newUsername) {
    //�����Ƿ��������û�����
	return username.compare(newUsername);
}
int user::checkRepeat(user temp) {
    //��¼���������û����Ƿ����ȷ ���򷵻�-1
    if(username.size()==temp.username.size()&&temp.password.size()==password.size()){
        if(username.compare(temp.username)==0&&password.compare(temp.password)==0)
        return 0;
        else return -1;
    }
    else return -1;
}
int user::changePasswd(string temp){
    //�޸�����
    this->password=temp;
    return SUCCESS;
}
int user::compareName(string otherName){
    //���ƻ���װ
	return name.compare(otherName);
}
//�û����ݿⲿ��
user* userList::getuser(int id){
    //�����û� �����û�ָ�� ���ڶ��û���ֱ�Ӳ���
    if(id<size)return &list[id];
    else return NULL;
}
int userList::getSize(){
    //���ƻ���װ ����ֱ�ӻ���û����ݿ��С
    return size;
}
int user::printData(ofstream& ofs) {
    //�û����ݿ����ݱ���
	ofs << id << ',' << character << ',' << username << ',' << name << ',' << addressSize << ',';
	int i;
	for (i = 0; i < addressSize; i++) {
		ofs << address[i] << ',';
	}
	ofs << password << ',' << phoneNumber << ',' << money << ',' << state << endl;
    return SUCCESS;
}
int userList::registerUser(string username, string name, string password, string phoneNumber) {
    //ע���û��������أ�
	int i;
	int judge = 0;
    cout<<"register!"<<endl;
	for (i = 0; i < size; i++) {
        if (list[i].checkRepeat(username) == 0) {//����
			judge = 1;
			return FAIL;
		}
	}
	user *newList = new user[size + 1];
	for (i = 0; i < size; i++) {
		newList[i] = list[i];
	}
    newList[i] = user(i, username, name, password, phoneNumber);
    //cout<<"Register:"<<i<<username<<name<<password<<phoneNumber<<endl;
    delete []list;//��ֹ�ڴ�й¶ ���·����ڴ�
    list=(user *)*(&newList);
    size++;
	return i;
}
int userList::saveData(char* dict) {
    //�����û�����
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
int userList::checkall(){
    //���ڱ�ݵؽ������û�������Ϊ�������
    this->outNum=this->size;
    delete this->output;
    int i;
    this->output=new int[this->size];
    for(i=0;i<this->size;i++){
        this->output[i]=i;
    }
    return SUCCESS;
}
int userList::searchByName(string name){
    //�û�������Ϊ�ؼ��ʵ�����
	int i,count;
    count=0;
    for(i=0;i<size;i++){
		if(list[i].compareName(name)==0)count++;
	}
	outNum=count;

	delete []output;
	output=new int[count];
    count=0;
	for(i=0;i<size;i++){
		if(list[i].compareName(name)==0){
			output[count]=i;
			count++;
		}
	}
    return count;
}
int userList::login(user temp) {//�ɹ�����ID�༴userList��Ӧλ�ã�ʧ�ܷ���Fail
    //�û���¼ ����Ϊ���û��������빹����ݴ��û�
	int i;
	int judge = 0;
    int save=0;
	for (i = 0; i < size; i++) {
        cout<<list[i].getName()<<endl;
        if (list[i].checkRepeat(temp) == 0) {
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
    //��ǰ��¼�û�����
    currentNum=num;
    cout<<num<<"set!"<<endl;
    return num;
}
int userList::getCurrentNum(){
    //��ȡ��ǰ��¼�û�
    return currentNum;
}
int userList::getAdmin(){
    //����ϵͳ����Ա������Ĭ��Ϊ0��������ʵ��ϵͳ����Աλ�����ݿ�����λ�ã�
    int i;
    for(i=0;i<this->size;i++){
        if(this->list[i].character==ADMIN_USER)return i;
    }
    return FAIL;
}
userList::userList(char* dict) {
    //����Ŀ¼�����û����ݿ�
	ifstream ifs;
    int i, j;
	int id_save;
	int character_save;
	string username_str_save;
	string name_str_save;
	string address_save[DEFAULT_SIZE];//����ӵ�ж����ַ 
	int addressSize_save;
	string password_save;
    string phoneNumber_save;
	int money_save;
	char common_save;
	int state_save;//��½״̬ 

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
		cout << "usr���ݿ��쳣�������飡" << endl;
	}
	else {
		size = length;
        delete [] list;
		list = new user[length];
		output=NULL;
		outNum=0;
		for (i = 0; i < length; i++) {
            if(ifs.fail()){
                cout << "usr���ݿ��쳣�������飡" << endl;
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
            cout << phoneNumber_save<<endl;
            cout << id_save << ',' << character_save << ',' << username_str_save << ',' << name_str_save << ',' << addressSize_save << ','<<password_save<<endl;
			list[i] = user(i, character_save, username_str_save, name_str_save, addressSize_save, address_save, password_save, phoneNumber_save, money_save, state_save);
		}
	}

}
//��ַ���������Ӣ�Ķ���"," 
string timeconvert(unsigned int time){
    //ʱ��ת�����������ٴ��븴���ʣ�ֻ��һ���Ĺ��ߺ���û�б�Ҫ�ٵ�������
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

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    string str1="��ã�";
    QString qss=QString::fromLocal8Bit(str1.data());
    QByteArray mm=qss.toLocal8Bit();
    string str=string(mm);
    cout<<str<<endl;
    login w;
    w.showMaximized();
    return a.exec();
}
