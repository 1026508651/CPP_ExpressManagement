#ifndef PKGMANREGISTER_H
#define PKGMANREGISTER_H

#include <QMainWindow>

namespace Ui {
class pkgManRegister;
}

class pkgManRegister : public QMainWindow
{
    Q_OBJECT

public:
    explicit pkgManRegister(QWidget *parent = nullptr);
    ~pkgManRegister();
    int checkPassword();
    virtual void resizeEvent(QResizeEvent * event);
    void closeEvent(QCloseEvent *e);

private slots:
    void on_Register_clicked();

private:
    Ui::pkgManRegister *ui;
};

#endif // PKGMANREGISTER_H
