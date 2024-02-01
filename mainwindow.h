#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <sqlite3.h>
#include <QMainWindow>
#include <QInputDialog>
#include <QDir>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class Person;
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

class Person {
public:
    Person(string id, string name, string phone);
    /*void setId(string id);
    void setName(string name);
    void setPhone(string phone);*/
    string getName();
    string getPhone();

private:
    string id;
    string name;
    string phone;
};

#endif // MAINWINDOW_H
