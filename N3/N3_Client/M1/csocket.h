#ifndef CSOCKET_H
#define CSOCKET_H

#include <QObject>
#include <QWebSocket>

class CSocket : public QObject
{
    Q_OBJECT
public:
    static CSocket *instance();
    QWebSocket m_webSocket;
    void onConnect(const QUrl &url);
    bool WaitForSignalFinished3(const char * signal, const unsigned int millisecond);
    void onSendMesssage(QString message);
signals:
    void endRecieved();
    void closed();
private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
private:
    static CSocket* m_instance;
    explicit CSocket(QObject *parent = nullptr);

    QUrl m_url;
};

#endif // CSOCKET_H
