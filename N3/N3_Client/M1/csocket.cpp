#include "csocket.h"
#include "basic.h"
#include <QMessageBox>
#include "QJsonDocument"
CSocket* CSocket::m_instance = nullptr;
extern QJsonObject recvBuffer;//接收缓冲区
extern int classType;//类种类
extern int funcNum;//函数序号
extern int recved;//单接收控制（防止阻塞导致丢包）
extern int sendable;//可发送控制（简单流量控制）
CSocket::CSocket(QObject *parent) : QObject(parent)
{
}

CSocket *CSocket::instance()//多页面共用一个Socket的方法：构造函数
{
    if (m_instance == nullptr)
        m_instance = new CSocket;
    return m_instance;
}

void CSocket::onConnect(const QUrl &url)//连接
{
    m_url = url;

    connect(&m_webSocket, &QWebSocket::connected, this, &CSocket::onConnected);//QWeksocket已连接信号和本类已连接槽函数链接
    connect(&m_webSocket, &QWebSocket::disconnected, this, &CSocket::closed);//QWebsocket断开连接信号和本类关闭槽函数连接

    m_webSocket.open(QUrl(url));
}

void CSocket::onConnected()
{
    qDebug()<<"connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &CSocket::onTextMessageReceived);//发送文件槽函数连接

}

void CSocket::onTextMessageReceived(QString message)//以字符串形式接收文件并转换成Json形式
{
    if(recved==FAIL){
        QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
        if(doc.object().value("classType").toInt()==classType&&doc.object().value("funcNum").toInt()==funcNum){
            recvBuffer= doc.object();
            recved=SUCCESS;
            sendable=FAIL;
            classType=FAIL;
            funcNum=FAIL;
            emit endRecieved();//接收数据结束，发送信号提示可以开启下次接受
        }
    }

}

void CSocket::onSendMesssage(QString message)
{

    m_webSocket.sendTextMessage(message);
    m_webSocket.flush();
    sendable=SUCCESS;
    qDebug()<<message;

}
