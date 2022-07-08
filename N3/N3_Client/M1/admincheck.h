#ifndef ADMINCHECK_H
#define ADMINCHECK_H

#include <QMainWindow>

namespace Ui {
class AdminCheck;
}

class AdminCheck : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);
    explicit AdminCheck(QWidget *parent = nullptr);
    ~AdminCheck();

private slots:
    void on_search_clicked();

    void on_safety_clicked();

private:
    Ui::AdminCheck *ui;
};

#endif // ADMINCHECK_H
