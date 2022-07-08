#include "variables.h"
#include "login.h"
#include <QTextCodec>
#include "sstream"
#include <QApplication>
static unsigned int Time = 0;
char userLocal[DEFAULT_SIZE] = "users.txt";//用户信息存储文件
char packageLocal[DEFAULT_SIZE] = "packages.txt";//快递信息存储文件
userList Users = userList(userLocal);//单机版初始化用户数据
packageList Packages = packageList(packageLocal);//单机版初始化快递数据
/*
 * Users和Packages作为extern全局变量，主要跑在Main.cpp里，目的是保证类的封装性良好
*/

//包裹部分
package::package(long long pkgNums){
    //寄送快递时仅给快递单号的包裹构造函数
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
    //默认包裹构造函数
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
    //文件读取信息建立包裹数据库的构造函数
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
    //收包裹函数：修改状态，加入收包裹时间
	status = PKG_RECV;
	time_t t;
	struct tm *tmp;
	time(&t);
	tmp = localtime(&t);
	//2204301710
	Time = tmp->tm_year % 100 * 100000000 + (tmp->tm_mon + 1) * 1000000 + (tmp->tm_mday) * 10000 + tmp->tm_hour * 100 + tmp->tm_min;
    //快递费全是整数 向下取整
    //收快递按需求 将一半的快递费转给快递员
    recvTime = Time;
    return SUCCESS;
}
int package::senderCompare(string someone){
    //为保证封装，对人名和收件人对比
	return sender.compare(someone);
}
int package::recverCompare(string someone) {
    //为保证封装，对人名和发件人对比
	return recver.compare(someone);
}
int package::packWrite(const string& recvName,string recvPhones,const string& recvAddress,const string& itemDess){
    //寄件时发件人写快递单
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
    //快递公司写快递单
	sender=senders;
	sendAdress=sendAddress;
	status=PKG_UNRECV;
	return status;
}
int package::printData(ofstream& ofs){
    //保存文件到文件流
	ofs<<pkgNum<<','<<status<<','<<sender<<','<<recvPhone<<','<<sendAdress<<','<<recvAdress<<','<<recver<<','<<sendTime<<','<<recvTime<<','<<itemDes<<endl;
    return SUCCESS;
}
string package::getStatus(){
    //翻译包裹状态为字符串，目的是保持封装
    string s;
    switch(this->status){
        case PKG_INIT:
        s=string("填写中");
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
//以下为不破坏封装所做的信息获取函数
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
//包裹数据库部分
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
//包裹搜索函数（逐个匹配）
int packageList::searchRecverBySender(string name,user& usr,string keyname){
    //根据用户权限（管理员/普通用户）返回跟自身相关的搜索结果 搜索接收人
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
    //根据用户权限（管理员/普通用户）返回跟自身相关的搜索结果 搜索发送人
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
    //用于收快递页面的直接搜索
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
    //通过时间搜索，支持区间搜索，实际运用仅运用了时间点（区间格式识别较为复杂）
    //分支：type表示搜索寄出时间/接收时间
    //其他分支为鉴权（管理员/普通用户）
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
    //构造函数，通过数据文件目录直接构造数据库
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
		cout << "pkg数据库异常，建议检查！" << endl;
	}
	else {
		size = length;
		list = new package[length];
		for (i = 0; i < length; i++) {
            if(ifs.fail()){
                cout << "pkg数据库异常，建议检查！" << endl;
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
    //保存数据 数据持久化
	ofstream ofs;
	ofs.open(dict, ios::out|ios::trunc);
	ofs<<size<<endl;
	int i;
	for (i = 0; i < size; i++) {
		list[i].printData(ofs);
	}
	return SUCCESS;
}
//用户函数
int user::addAdress(string newAdress) {
    //用户增加地址
	if (addressSize < DEFAULT_SIZE) {
		address[addressSize] = newAdress;
		addressSize++;
        return SUCCESS;
	}
	else return FAIL;
}
int user::checkPasswd(string temp){
    //用户修改密码 需要一定条件才能调用此函数 因此函数只实现修改密码函数
    return temp.compare(this->password);
}
string user::getName(){
    //获取用户名（不破坏封装性 ）只有登录用户才能使用
	if(state==USER_LOGGED)return name;
    else return "";
}
long long user::sendPkg(long long pkgNum/*分配单号*/, int num/*选择第几个地址，默认为0*/,string recvName,int recvNum,string recvPhone,string itemDes) {
    //发送快递 将输入信息进行归档
    if (state == USER_LOGGED&&this->money>=PACK_FEE) {
		//获取各项信息
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
    //不破坏封装的获取信息函数
    return this->username;
}
user::user() {
    //用户的默认构造函数（用于在尚未获取用户信息时的暂时构造）
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
    //读取文件数据库的用户构造函数
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
    //用户注册时的后台用户构造初始化函数
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
    //类似包裹的getStatus函数
    switch(this->character){
        case NORMAL_USER:
            return string("用户");
        case ADMIN_USER:
            return string("管理员");
    }
    return "";
}
int user::getMoney() {
    //不破坏封装 防止对Money进行直接修改
	return money;
}
int user::addMoney(int num) {
    //支持负数 改变目前账户钱数
	money += num;
	return money;
}
int user::payMoney(int num){
    //该函数更加直接 付钱
    money -= num;
    return money;
}
int user::recvPkg(package pack) {
    //收快递 建立用户和快递之间的关系
	Packages.list[pack.pkgNum].recved();
    return SUCCESS;
}
int user::checkRepeat(string newUsername) {
    //鉴定是否有重名用户出现
	return username.compare(newUsername);
}
int user::checkRepeat(user temp) {
    //登录检测密码和用户名是否均正确 否则返回-1
    if(username.size()==temp.username.size()&&temp.password.size()==password.size()){
        if(username.compare(temp.username)==0&&password.compare(temp.password)==0)
        return 0;
        else return -1;
    }
    else return -1;
}
int user::changePasswd(string temp){
    //修改密码
    this->password=temp;
    return SUCCESS;
}
int user::compareName(string otherName){
    //不破坏封装
	return name.compare(otherName);
}
//用户数据库部分
user* userList::getuser(int id){
    //查找用户 返回用户指针 用于对用户的直接操作
    if(id<size)return &list[id];
    else return NULL;
}
int userList::getSize(){
    //不破坏封装 用于直接获得用户数据库大小
    return size;
}
int user::printData(ofstream& ofs) {
    //用户数据库数据保存
	ofs << id << ',' << character << ',' << username << ',' << name << ',' << addressSize << ',';
	int i;
	for (i = 0; i < addressSize; i++) {
		ofs << address[i] << ',';
	}
	ofs << password << ',' << phoneNumber << ',' << money << ',' << state << endl;
    return SUCCESS;
}
int userList::registerUser(string username, string name, string password, string phoneNumber) {
    //注册用户（含查重）
	int i;
	int judge = 0;
    cout<<"register!"<<endl;
	for (i = 0; i < size; i++) {
        if (list[i].checkRepeat(username) == 0) {//查重
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
    delete []list;//防止内存泄露 重新分配内存
    list=(user *)*(&newList);
    size++;
	return i;
}
int userList::saveData(char* dict) {
    //保存用户数据
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
    //用于便捷地将所有用户索引作为搜索结果
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
    //用户姓名作为关键词的搜索
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
int userList::login(user temp) {//成功返回ID亦即userList对应位置，失败返回Fail
    //用户登录 参数为以用户名和密码构造的暂存用户
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
    //当前登录用户设置
    currentNum=num;
    cout<<num<<"set!"<<endl;
    return num;
}
int userList::getCurrentNum(){
    //获取当前登录用户
    return currentNum;
}
int userList::getAdmin(){
    //查找系统管理员索引（默认为0，但可以实现系统管理员位于数据库任意位置）
    int i;
    for(i=0;i<this->size;i++){
        if(this->list[i].character==ADMIN_USER)return i;
    }
    return FAIL;
}
userList::userList(char* dict) {
    //根据目录构造用户数据库
	ifstream ifs;
    int i, j;
	int id_save;
	int character_save;
	string username_str_save;
	string name_str_save;
	string address_save[DEFAULT_SIZE];//允许拥有多个地址 
	int addressSize_save;
	string password_save;
    string phoneNumber_save;
	int money_save;
	char common_save;
	int state_save;//登陆状态 

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
		cout << "usr数据库异常，建议检查！" << endl;
	}
	else {
		size = length;
        delete [] list;
		list = new user[length];
		output=NULL;
		outNum=0;
		for (i = 0; i < length; i++) {
            if(ifs.fail()){
                cout << "usr数据库异常，建议检查！" << endl;
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
//地址不允许出现英文逗号"," 
string timeconvert(unsigned int time){
    //时间转换函数，减少代码复用率，只有一个的工具函数没有必要再单独成类
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
    string str1="你好！";
    QString qss=QString::fromLocal8Bit(str1.data());
    QByteArray mm=qss.toLocal8Bit();
    string str=string(mm);
    cout<<str<<endl;
    login w;
    w.showMaximized();
    return a.exec();
}
