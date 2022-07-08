#include "serverface.h"
#include "ui_serverface.h"
#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QHostAddress>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <variables.h>
#include <QJsonDocument>
static int ableSend=0;
static int mngIDCount=0;
extern QString sendBuffer;
extern QString recvBuffer;
QString recvBuffer;
QString sendBuffer;
extern bool send;
ServerFace::ServerFace(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerFace)
{
    ui->setupUi(this);
    server=new QWebSocketServer("Server",QWebSocketServer::NonSecureMode,this);
    connect(server,&QWebSocketServer::newConnection,this,&ServerFace::onNewConnection);

    connect(ui->listenBtn,&QPushButton::clicked,[this](){
        if(ui->listenBtn->text()!="Listen"){
            ui->listenBtn->setText("Listen");
            clearClient();
            Users.saveData(userLocal);//停止监听或关闭窗口都会保存数据
            Packages.saveData(packageLocal);
            server->close();
            ableSend=-1;
        }else{
            QHostAddress address;
            address=QHostAddress::Any;

            //判断是否连接上
            if(server->listen(address,12345)){
                ableSend=0;
                ui->listenBtn->setText("Dislisten");
                ui->recv->append(timeDebug("Listenning"));
            }
        }
    });



}

ServerFace::~ServerFace()

    {
        clearClient();
        server->close();
        delete ui;
    }
void ServerFace::clearClient()
{
    for(int i=clientList.count()-1;i>=0;i--)
    {
        //qDebug()<<i;
        clientList.at(i)->disconnect();
        clientList.at(i)->close();
    }
    qDeleteAll(clientList);
    clientList.clear();
}
void ServerFace::onNewConnection(){
    QWebSocket *socket=server->nextPendingConnection();
    if(!socket)
        return;
    clientList.push_back(socket);
    ui->send->append(timeDebug(QString("[New Connect] Address:%1  Port:%2").arg(socket->peerAddress().toString())
                         .arg(socket->peerPort())));

    //收到消息

    connect(socket,&QWebSocket::textMessageReceived,[this](const QString &msg){
        ui->recv->append(timeDebug("[Recv]:"+msg));
        recvBuffer=msg;

        dataFragment* recvMsg=new dataFragment(QString::fromLocal8Bit(msg.toLocal8Bit().data()));
        socketManager* manager=new socketManager(mngIDCount++);
        QJsonObject review=manager->recvMesgProcess(*recvMsg);
        if(review.contains("need")){
            review.remove("need");
            review.insert("funcNum",recvMsg->funcNum);
            review.insert("classType",recvMsg->classType);
            QString strRet;
            QJsonDocument doc(review);
            strRet = QString(doc.toJson(QJsonDocument::Compact));
            ui->send->append(timeDebug("[Send]:"+strRet));
            sendMessage(strRet);
        }
        //对Json进行处理并调用
    });
    //发送消息

    //断开连接，释放
    connect(socket,&QWebSocket::disconnected,[this,socket](){
        clientList.removeAll(socket);
        socket->deleteLater();
    });
}
void ServerFace::sendMessage(const QString &text){//发送数据（多线程给所有客户端都发送）
    QString data_str;
    data_str = text;
    for (int i = 0; i < clientList.length(); i++)
        {
            clientList[i]->sendTextMessage(data_str);
            clientList[i]->flush();
        }
}
QString ServerFace::timeDebug(QString original){//显示收/发数据的时间
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString time=current_date_time.toString();
    return QString("<"+time+">"+original);
}
void ServerFace::closeEvent(QCloseEvent *e){
    Users.saveData(userLocal);
    Packages.saveData(packageLocal);//关闭窗口或停止监听都会保存数据
}
