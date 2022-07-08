#ifndef PKGSEARCH_H
#define PKGSEARCH_H

#include <QMainWindow>

namespace Ui {
class pkgSearch;
}

class pkgSearch : public QMainWindow
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);
    explicit pkgSearch(QWidget *parent = nullptr);
    ~pkgSearch();

private slots:
    void on_Get_clicked();

private:
    Ui::pkgSearch *ui;
};

#endif // PKGSEARCH_H
