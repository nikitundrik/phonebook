#include <iostream>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

using namespace std;

vector<string> result;

static int callback(void *notUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        const char *compRes = argv[i] ? argv[i] : "none";
        cout << azColName[i] << " " << compRes << endl;
    }
    cout << endl;
    return 0;
}

static int loadEntries(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << argv[i] << endl;
        result.push_back(string(argv[i]));
    }
    cout << "end" << endl;
    return 0;
}

void MainWindow::addEntry() {
    bool success;
    QString name = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Name:"), QLineEdit::Normal,
                                         QDir::home().dirName(), &success);
    QString phone = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                          tr("Phone number:"), QLineEdit::Normal,
                                          QDir::home().dirName(), &success);
    char sql[500];
    QByteArray nameBa = name.toLocal8Bit();
    QByteArray phoneBa = phone.toLocal8Bit();
    char *nameChar = nameBa.data();
    char *phoneChar = phoneBa.data();
    int n;
    n = snprintf(sql, 500, "INSERT INTO users(name, phone) VALUES ('%s', '%s');", nameChar, phoneChar);
    rc = sqlite3_exec(db, sql, callback, 0, &dbErr);
    if (rc != SQLITE_OK) {
        cout << "error: " << dbErr << endl;
        sqlite3_free(dbErr);
    } else {
        cout << "success" << endl;
    }
    result.clear();
    showEntries();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dbErr = 0;
    rc = sqlite3_open("users.db", &db);
    if (rc) {
        cout << "error:" << sqlite3_errmsg(db) << endl;
    } else {
        cout << "opened" << endl;
    }
    openDb();
    showEntries();
    connect(ui->actionAdd, &QAction::triggered, [=]() {
        addEntry();
    });
}

void MainWindow::openDb() {
    char *sql = "CREATE TABLE IF NOT EXISTS users(" \
        "id     INTEGER     PRIMARY KEY     AUTOINCREMENT," \
        "name   TEXT                    NOT NULL," \
        "phone  TEXT                    NOT NULL" \
        ");";
    rc = sqlite3_exec(db, sql, callback, 0, &dbErr);
    if (rc != SQLITE_OK) {
        cout << "error: " << dbErr << endl;
        sqlite3_free(dbErr);
    } else {
        cout << "success" << endl;
    }
}

void MainWindow::showEntries() {
    char *sql = "SELECT * FROM users";
    const char *data = "sam";
    rc = sqlite3_exec(db, sql, loadEntries, (void*)data, &dbErr);
    if (rc != SQLITE_OK) {
        cout << "error: " << dbErr << endl;
        sqlite3_free(dbErr);
    } else {
        cout << "success" << endl;
    }
    int toCreate = 0;
    int column = 0;
    ui->entryTable->setRowCount(0);
    for (string entry: result) {
        if (toCreate == 0) {
            ui->entryTable->setRowCount(ui->entryTable->rowCount() + 1);
            column++;
        } else {
            QString itemText = QString::fromStdString(entry);
            cout << itemText.toStdString() << endl;
            QTableWidgetItem *tableItem = new QTableWidgetItem(itemText);
            cout << toCreate - 1 << " " << column - 1 << endl;
            ui->entryTable->setItem(column - 1, toCreate - 1, tableItem);
        }
        toCreate++;
        if (toCreate == 3) {
            toCreate = 0;
        }
    }
}

void MainWindow::search() {
    QString searchQuery = ui->searchEdit->text();
    ui->entryTable->setRowCount(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    sqlite3_close(db);
}
