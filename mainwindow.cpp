#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //model = new DataModel(QString("/home/steve/testdata"), true);
    model = new DataModel();
    //model->addPath("/home/steve/testdata", true, nullptr);
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

void MainWindow::on_pathEdit_returnPressed()
{
    model->addPath(ui->pathEdit->text(), true, 0);
    //ui->treeView->reset();
}

void MainWindow::on_addButton_clicked()
{
    model->addPath(ui->pathEdit->text(), true, 0);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    ui->duplicateList->clear();

    ui->duplicateList->addItems(model->listDuplicates(index));
}

void MainWindow::on_treeView_entered(const QModelIndex &index)
{
    ui->duplicateList->clear();

    ui->duplicateList->addItems(model->listDuplicates(index));
}

void MainWindow::on_treeView_activated(const QModelIndex &index)
{
    ui->duplicateList->clear();

    ui->duplicateList->addItems(model->listDuplicates(index));
}


void MainWindow::on_treeView_pressed(const QModelIndex &index)
{
    ui->duplicateList->clear();

    ui->duplicateList->addItems(model->listDuplicates(index));
}

void MainWindow::on_pushButton_clicked()
{
    // Determine which duplicates have been selected

    // For each selection, call a function in the model that deletes a file
    // based on the ModelIndex of the file to be kept and the filename of
    // the selection
}
