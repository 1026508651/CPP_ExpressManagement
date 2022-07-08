#ifndef USERSSAFTY_H
#define USERSSAFTY_H

#include <QMainWindow>

namespace Ui {
class usersSafty;
}

class usersSafty : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);
    explicit usersSafty(QWidget *parent = nullptr);
    ~usersSafty();

private slots:
    void on_Changepasswd_2_clicked();

private:
    Ui::usersSafty *ui;
};

#endif // USERSSAFTY_H
