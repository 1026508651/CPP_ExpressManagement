#ifndef NAVIGATION_H
#define NAVIGATION_H
#include "variables.h"
#include <QMainWindow>
extern packageList Packages;
extern userList Users;
namespace Ui {
class Navigation;
}

class Navigation : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    explicit Navigation(QWidget *parent = nullptr);
    ~Navigation();
    void resizeEvent(QResizeEvent* event);
private slots:
    void on_search_clicked();

    void on_recv_clicked();

    void on_send_clicked();

    void on_safety_clicked();

    void on_account_clicked();

private:
    Ui::Navigation *ui;
};

#endif // NAVIGATION_H
