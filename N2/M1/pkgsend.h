#ifndef PKGSEND_H
#define PKGSEND_H
#include "variables.h"
#include <QMainWindow>
extern userList Users;
extern packageList Packages;

namespace Ui {
class pkgSend;
}

class pkgSend : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    explicit pkgSend(QWidget *parent = nullptr);
    ~pkgSend();
    void resizeEvent(QResizeEvent* event);
private slots:
    void on_adressAdd_clicked();

    void on_Send_clicked();

private:
    Ui::pkgSend *ui;
};

#endif // PKGSEND_H
