#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DataModel *model = new DataModel(QString("/home/steve/testdata"), true);
    //DataModel *model = new DataModel(QString("/media/pix/pix/Scifi/cons/DragonCon2013"), true);
    //model.debugDuplicates();
    //QFileSystemModel *model = new QFileSystemModel;
    //model->setRootPath(QDir::currentPath());

    //model->dumpFiles();


    ui->setupUi(this);

    ui->treeView->setModel((QAbstractItemModel *) model);


#if 0
    // Tree views display all rows and columns that are children of the root index, and looks like a table
    ui->treeView->setModel((QAbstractItemModel *) model);

    // List views display only column 0 from each row that is a child of the root index
    ui->listView->setModel((QAbstractItemModel *) model);

    // Column views display only column 0 from each row that is a child of the root index
    ui->columnView->setModel((QAbstractItemModel *) model);

    // Table views display all rows and columns that are children of the root index
    ui->tableView->setModel((QAbstractItemModel *) model);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}
