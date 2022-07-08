#ifndef ADMINPKGSEARCH_H
#define ADMINPKGSEARCH_H

#include <QMainWindow>

namespace Ui {
class adminPkgSearch;
}

class adminPkgSearch : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);
    explicit adminPkgSearch(QWidget *parent = nullptr);
    ~adminPkgSearch();

private slots:
    void on_Get_clicked();

private:
    Ui::adminPkgSearch *ui;
};

#endif // ADMINPKGSEARCH_H
