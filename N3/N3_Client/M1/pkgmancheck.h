#ifndef PKGMANCHECK_H
#define PKGMANCHECK_H

#include <QMainWindow>

namespace Ui {
class pkgManCheck;
}

class pkgManCheck : public QMainWindow
{
    Q_OBJECT

public:
    explicit pkgManCheck(QWidget *parent = nullptr);
    ~pkgManCheck();
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);

private slots:
    void on_Get_2_clicked();

private:
    Ui::pkgManCheck *ui;
};

#endif // PKGMANCHECK_H
