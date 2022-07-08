#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    int checkPassword();
    virtual void resizeEvent(QResizeEvent * event);
    ~RegisterWindow();
    void closeEvent(QCloseEvent *e);

private slots:
    void on_Register_clicked();

private:
    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H
