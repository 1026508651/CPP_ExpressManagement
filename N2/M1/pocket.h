#ifndef POCKET_H
#define POCKET_H

#include <QMainWindow>

namespace Ui {
class pocket;
}

class pocket : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);
    explicit pocket(QWidget *parent = nullptr);
    ~pocket();

private slots:
    void on_addMoney_clicked();

private:
    Ui::pocket *ui;
};

#endif // POCKET_H
