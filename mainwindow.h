#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlDriverPlugin>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtCore>
#include <QSqlTableModel>
#include <QSqlQueryModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool createdbconnection();
    QSqlDatabase db;
    QStringList Sl;
    QStringListModel *Slm;
    //QSqlTableModel *model;
    QSqlQueryModel *queryModel;

private slots:
    void Update();
    void New();
    void Select(QModelIndex _model);
    void Delete();
    void on_NewTable_clicked();
};

#endif // MAINWINDOW_H
