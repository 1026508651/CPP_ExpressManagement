user getCurrentUser()
void setCurrentNum(int num);
void refreshCurrentUser(user);
Packages.outcome+Packages.outcomeNum packagesSearch(searchType,||currentUser||,searchName)
Users.outcome+Users.outcomNum usersSearch(searchType,keyWord)
user Users.getUser(int num)
Users.userDelete(int num)
void Users.saveData();
void Packages.saveData();
int Users.getSize();
//回复和请求需要有校验号作为支持
int Users.login(user tempLogin);
