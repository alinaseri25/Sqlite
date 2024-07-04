#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->Btn_Update,SIGNAL(clicked()),this,SLOT(Update()));
    connect(ui->Btn_New,SIGNAL(clicked()),this,SLOT(New()));
    connect(ui->Btn_Delete,SIGNAL(clicked()),this,SLOT(Delete()));
    /*QMessageBox::about(this,QString("Title"),QString("DIR:%1\r\nDisplayName:%2\r\nFilePath:%3").arg(QApplication::applicationDirPath())
                       .arg(QApplication::applicationDisplayName())
                       .arg(QApplication::applicationFilePath()));*/
    if(createdbconnection())
    {
        statusBar()->showMessage(QString("Database Connected ..."),3000);
    }
    else
    {
        QMessageBox::information(this,QString("Connection ERROR"),QString("I Can't Connect To DB\nSorry About It!"));
        statusBar()->showMessage(QString("I Can't Connect To DB\nSorry About It!"),5000);
        close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::createdbconnection()
{
    QString Str = QCoreApplication::applicationDirPath();
    db = QSqlDatabase::addDatabase(QString("QSQLITE"),QString("Connection"));
    Str = QString("%1/TestDb").arg(QApplication::applicationDirPath());
    db.setDatabaseName(Str);

    if(db.open())
    {
        /*QSqlQuery query(db);
        query.prepare("SELECT * FROM Users");
        if(!query.exec())
        {
            return false;
        }*/
        /*model = new QSqlTableModel(this,db);
        model->setTable(QString("Users"));
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->select();

        model->setHeaderData(0,Qt::Horizontal,QString("ردیف"));
        model->setHeaderData(1,Qt::Horizontal,QString("نام"));
        model->setHeaderData(2,Qt::Horizontal,QString("نام خانوادگی"));
        model->setHeaderData(3,Qt::Horizontal,QString("نام ورود"));
        model->setHeaderData(4,Qt::Horizontal,QString("گذر واژه"));*/

        queryModel = new QSqlQueryModel(this);

        QSqlQuery query(db);
        query.prepare("SELECT id,name,family FROM Users");
        if(!query.exec())
        {
            return false;
        }
        queryModel->setQuery(query);

        queryModel->setHeaderData(0,Qt::Horizontal,QString("ردیف"));
        queryModel->setHeaderData(1,Qt::Horizontal,QString("نام"));
        queryModel->setHeaderData(2,Qt::Horizontal,QString("نام خانوادگی"));

        ui->TbV_Gride->setModel(queryModel);
        ui->TbV_Gride->resizeColumnsToContents();
        ui->TbV_Gride->setLayoutDirection(Qt::RightToLeft);
        ui->TbV_Gride->setSelectionBehavior(QAbstractItemView::SelectRows);

        connect(ui->TbV_Gride,SIGNAL(clicked(QModelIndex)),this,SLOT(Select(QModelIndex)));

        /*while (query.next()) {
            ui->Txt_id->setText(query.value(0).toString());
            ui->Txt_Name->setText(query.value(1).toString());
            ui->Txt_Family->setText(query.value(2).toString());
            ui->Txt_Username->setText(query.value(3).toString());
            ui->Txt_Pass->setText(query.value(4).toString());
            Sl << query.value(0).toString();
        }*/
        db.close();
        return true;
    }
    else
        return false;
}

void MainWindow::Update()
{
    if(db.open())
    {
        QSqlQuery query(db);
        QString Str = QString("UPDATE Users SET name = ?,family = ?,username = ?,pass = ? WHERE id = ?");
        query.prepare(Str);
        query.addBindValue(ui->Txt_Name->text());
        query.addBindValue(ui->Txt_Family->text());
        query.addBindValue(ui->Txt_Username->text());
        query.addBindValue(ui->Txt_Pass->text());
        query.addBindValue(ui->Txt_id->text());
        if(!query.exec())
        {
            QMessageBox::information(this,QString("Error"),QString("Error in Database"));
            statusBar()->showMessage(QString("Error in Database"),3000);
        }
        else
        {
            statusBar()->showMessage(QString("Update Complete Successfuly ..."),3000);
        }
        //model->select();
        query.prepare("SELECT id,name,family FROM Users");
        query.exec();
        queryModel->setQuery(query);
        db.close();
    }
    else
    {
        QMessageBox::information(this,QString("Error"),QString("Error in Database"));
        statusBar()->showMessage(QString("Error in Database"),3000);
    }
}

void MainWindow::New()
{
    if(ui->Btn_New->text() == "Insert")
    {
        ui->Btn_New->setText("New");
        ui->Btn_Update->setEnabled(true);
        if(db.open())
        {
            QString Str = QString("INSERT INTO Users (id,name,family,username,pass) VALUES (?,?,?,?,?)");
            QSqlQuery query(db);
            query.prepare(Str);
            query.addBindValue(ui->Txt_id->text());
            query.addBindValue(ui->Txt_Name->text());
            query.addBindValue(ui->Txt_Family->text());
            query.addBindValue(ui->Txt_Username->text());
            query.addBindValue(ui->Txt_Pass->text());
            if(query.exec())
            {
                statusBar()->showMessage(QString("Insert Completed successfuly ..."),3000);
            }
            else
            {
                QMessageBox::information(this,QString("Error"),QString("Error in Database"));
                statusBar()->showMessage(QString("Error in Database"),3000);
            }
            query.prepare("SELECT id,name,family FROM Users");
            query.exec();
            queryModel->setQuery(query);
        }
        else
        {
            QMessageBox::information(this,QString("Error"),QString("Error in Database"));
            statusBar()->showMessage(QString("Error in Database"),3000);
        }
    }
    else
    {
        ui->Btn_Update->setEnabled(false);
        ui->Btn_New->setText("Insert");
        ui->Txt_Name->setText("");
        ui->Txt_Family->setText("");
        ui->Txt_Username->setText("");
        ui->Txt_Pass->setText("");
        if(db.open())
        {
            QSqlQuery query(db);
            query.prepare("select MAX(id) from users");
            if(query.exec())
            {
                int i = 0;
                if(query.next())
                {
                    i = query.value(0).toInt();
                }
                ++i;
                ui->Txt_id->setText(QString::number(i));
                statusBar()->showMessage(QString("Database Ready To Insert New Data ..."),3000);
            }
            else
            {
                QMessageBox::information(this,QString("Error"),QString("Error in Database"));
                statusBar()->showMessage(QString("Error in Database"),3000);
            }
            query.prepare("SELECT id,name,family FROM Users");
            query.exec();
            queryModel->setQuery(query);
        }
        else
        {
            QMessageBox::information(this,QString("Error"),QString("Error in Database"));
            statusBar()->showMessage(QString("Error in Database"),3000);
        }
    }
    //model->select();
    db.close();
}

void MainWindow::Select(QModelIndex _model)
{
    /*ui->Btn_New->setText("New");
    ui->Btn_Update->setEnabled(true);
    QString STR = model->index(_model.row(),0).data(Qt::DisplayRole).toString();
    if(db.open())
    {
        QSqlQuery query(db);
        query.prepare(QString("SELECT * FROM Users WHERE id = ?"));
        query.addBindValue(STR);
        if(query.exec())
        {
            if(query.next())
            {
                ui->Txt_id->setText(query.value(0).toString());
                ui->Txt_Name->setText(query.value(1).toString());
                ui->Txt_Family->setText(query.value(2).toString());
                ui->Txt_Username->setText(query.value(3).toString());
                ui->Txt_Pass->setText(query.value(4).toString());
            }
            statusBar()->showMessage(QString("New Record Selected ..."),3000);
        }
        else
        {
            QMessageBox::information(this,QString("Error"),QString("Error in Database"));
            statusBar()->showMessage(QString("New Record Not Selected ..."),3000);
        }
        db.close();
    }
    else
    {
        QMessageBox::information(this,QString("Error"),QString("Error in Database"));
        statusBar()->showMessage(QString("Error in Database"),3000);
    }*/
}

void MainWindow::Delete()
{
    ui->Btn_New->setText("New");
    ui->Btn_Update->setEnabled(true);
    if(db.open())
    {
        QSqlQuery query(db);
        QString str = QString("DELETE FROM Users WHERE id = ?");
        query.prepare(str);
        query.addBindValue(ui->Txt_id->text());
        if(query.exec())
        {
            statusBar()->showMessage(QString("Record Deleted ..."),3000);
        }
        else
        {
            QMessageBox::information(this,QString("Error"),QString("Error in Database"));
            statusBar()->showMessage(QString("Record Cannot Delete ..."),3000);
        }
        query.prepare("SELECT id,name,family FROM Users");
        query.exec();
        queryModel->setQuery(query);
    }
    //model->select();
    db.close();
}

void MainWindow::on_NewTable_clicked()
{
    if(db.open())
    {
        QSqlQuery query(db);
        query.prepare("CREATE TABLE Users(ID INT PRIMARY KEY NOT NULL"
                      ",name varchar(50)"
                      ",Family varchar(50)"
                      ",username varchar(50)"
                      ",pass varchar(50))");
        if(!query.exec())
        {
            QMessageBox::information(this,QString("Error"),QString("Error in Database"));
            statusBar()->showMessage(QString("Cannot Create Table ..."),3000);
        }
        else
        {
            statusBar()->showMessage(QString("Table Created ..."),3000);
        }
    }
    db.close();
    if(createdbconnection())
    {
        statusBar()->showMessage(QString("Database Connected ..."),3000);
    }
    else
    {
        QMessageBox::information(this,QString("Connection ERROR"),QString("I Can't Connect To DB\nSorry About It!"));
        statusBar()->showMessage(QString("I Can't Connect To DB\nSorry About It!"),5000);
        close();
    }
}
