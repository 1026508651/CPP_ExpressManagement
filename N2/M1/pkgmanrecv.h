#ifndef PKGMANRECV_H
#define PKGMANRECV_H

#include <QMainWindow>

namespace Ui {
class pkgManRecv;
}

class pkgManRecv : public QMainWindow
{
    Q_OBJECT

public:
    explicit pkgManRecv(QWidget *parent = nullptr);
    ~pkgManRecv();
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);

private slots:
    void on_Get_clicked();

private:
    Ui::pkgManRecv *ui;
};

#endif // PKGMANRECV_H
