#include "variables.h"
#ifndef PKGREV_H
#define PKGREV_H

#include <QMainWindow>

namespace Ui {
class pkgRev;
}

class pkgRev : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);
    explicit pkgRev(QWidget *parent = nullptr);
    ~pkgRev();

private slots:

    void on_Get_clicked();

private:
    Ui::pkgRev *ui;
};

#endif // PKGREV_H
