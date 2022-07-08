
#include <QMainWindow>
#include "variables.h"
namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    virtual void resizeEvent(QResizeEvent * event);
    explicit login(QWidget *parent = nullptr);
    ~login();
    void closeEvent(QCloseEvent *e);
private slots:
    void on_register_2_clicked();

    void on_login_2_clicked();


private:
    Ui::login *ui;
};

