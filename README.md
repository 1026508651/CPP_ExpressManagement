#  Express Management System

### @ZXHE

### Introduction

Based on Qt, this project simulated an express management system with three versions from the simplest one to the net version based on weksocket protocol. It is built by Mingw builder.

### Organization and  Versions

There are three versions of this management system.

| Version | Basic Functions | Advanced Functions | Network |
| ------- | --------------- | ------------------ | ------- |
| N1      | √               | ×                  | ×       |
| N2      | √               | √                  | ×       |
| N3      | √               | √                  | √       |

Furthermore, N3 was organized based on C-S structure with client and server.

**Basic Functions**: Users Management(Normal User and Administrator) (login, register and change password), Package Management(Send, Receive and search by keywords(send time, receive time, sender, receiver and package number)), Account Management(get, add and cut down when sending package) Database Management(Using basic txt file to save the data in the form of table. Saving data every time when the login window is closed).

**Advanced Functions**: Expressman(Package man in program) Management (add only by administrator as a user) Sending Process Change(Package man needs to send the package after the user send it and then the user can receive it). Account Management(Package man can earn money from the administrator after the package has been received) 

**Network**: Based on websocket and Qwebsocket(a library of Qt to make websocket easier to use), every invocation in the client to invoke a function that will modify the data in the database will be converted into a request datagram(shown in the file `basic.h`). The server will convert this datagram into the original request to invoke corresponding function with the data offered in the request. All data will be transmitted in the form of `json`.

### Deployment

language: `C++`

IDE: Qt Creator  4.8.0

Qt Version: V5.12.0 Mingw

ATTENTION:

1.Due to the complexity of Chinese characters encoding(mainly between UTF-8 and GBK), please do not open this project with MSVC2017(VS2017) or most of the output and characters in all windows will be garbled code.

### Structure

---

├── README.md

├── N1

​	├── M1//project file

​		.......

​		├── M1.pro//Qt Project File

​		.......

​	├── N1 Release//release version

​		.......

​		├── M1.exe

​		.......

├── N2

​	├── M1//project file

​		.......

​		├── M1.pro//Qt Project File

​		.......

​	├── N2 Release//release version

​		.......

​		├── M1.exe

​		.......

├── N3

​	├── N3_Client

​			├── M1//project file

​				.......

​				├── M1.pro//Qt Project File

​				.......

​			├── N3 CRelease//release version

​				.......

​				├── M1.exe

​				.......

​		├── N3_Server

​			├── N3_Server//project file

​				.......

​				├── N3_Server.pro//Qt Project File

​				.......

​			├── N3 Server Release//release version

​				.......

​				├── N3_Server.exe

​				.......

### Usage

​    Since this is a program with GUI, there is no need to learn how to use but following the logic and usage of ordinary software is enough.

### Reference

1. QT 
