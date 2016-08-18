#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //QModelIndex indexA, indexB, indexBChild, rootIndex;
    //bool rc;
    DataModel model(QString("/home/steve/testdata"), true);
    //DataModel model(QString("/media/pix/pix/Scifi/cons/DragonCon2013"), true);
    model.debugDuplicates();

    ui->setupUi(this);

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
