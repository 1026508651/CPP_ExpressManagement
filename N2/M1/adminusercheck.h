#ifndef ADMINUSERCHECK_H
#define ADMINUSERCHECK_H

#include <QMainWindow>

namespace Ui {
class adminUserCheck;
}

class adminUserCheck : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);
    explicit adminUserCheck(QWidget *parent = nullptr);
    ~adminUserCheck();

private slots:
    void on_Get_clicked();

    void on_delete_2_clicked();

    void on_addPkgMan_clicked();

    void on_mainTable_doubleClicked(const QModelIndex &index);

private:
    Ui::adminUserCheck *ui;
};

#endif // ADMINUSERCHECK_H
