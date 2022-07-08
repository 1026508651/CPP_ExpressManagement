#ifndef PKGMANNAVI_H
#define PKGMANNAVI_H

#include <QMainWindow>

namespace Ui {
class pkgManNavi;
}

class pkgManNavi : public QMainWindow
{
    Q_OBJECT

public:
    explicit pkgManNavi(QWidget *parent = nullptr);
    ~pkgManNavi();
    void closeEvent(QCloseEvent *e);
    void resizeEvent(QResizeEvent* event);

private slots:

    void on_search_clicked();

    void on_check_clicked();

    void on_user_2_clicked();

private:
    Ui::pkgManNavi *ui;
};

#endif // PKGMANNAVI_H
