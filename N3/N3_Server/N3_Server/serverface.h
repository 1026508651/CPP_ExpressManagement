#ifndef SERVERFACE_H
#define SERVERFACE_H
#include <QWebSocketServer>
#include <QMainWindow>
#include <QWebSocket>
namespace Ui {
class ServerFace;
}

class ServerFace : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerFace(QWidget *parent = nullptr);
    ~ServerFace();
    QJsonObject QstringToJson(QString jsonString);
    void sendMessage(const QString &text);
    void closeEvent(QCloseEvent *e);
    QString timeDebug(QString original);
private:
    void clearClient();
    void onNewConnection();

private:
    Ui::ServerFace *ui;

    QWebSocketServer *server;
    QList<QWebSocket*> clientList;
};

#endif // SERVERFACE_H
