#ifndef SAFETY_H
#define SAFETY_H

#include <QMainWindow>

namespace Ui {
class Safety;
}

class Safety : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);
    explicit Safety(QWidget *parent = nullptr);
    ~Safety();

private slots:
    void on_Changepasswd_clicked();

private:
    Ui::Safety *ui;
};

#endif // SAFETY_H
