#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <sqlite3.h>
#include <QMainWindow>
#include <QInputDialog>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void openDb();
    void showEntries();
    void addEntry();
    void search();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    sqlite3 *db;
    char *dbErr;
    int rc;
};
#endif // MAINWINDOW_H
