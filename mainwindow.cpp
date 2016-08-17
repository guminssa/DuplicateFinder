#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //QModelIndex indexA, indexB, indexBChild, rootIndex;
    //bool rc;
    DataModel model(QString("/home/steve"), true);

    ui->setupUi(this);

    // Apparently, QStandardItemModel requires all entries to be children of the root index

    //model = new QStandardItemModel(1, 3, this);
    //model = new DataModel(3, 3, 0);
    //model->setHeaderData(0, Qt::Horizontal, "Column 0", Qt::DisplayRole);

    //model->insertRow(0);
    /*
    rootIndex = QModelIndex();
    indexA = this->model->index(0, 0, rootIndex);
    model->setData(indexA, "A", Qt::DisplayRole);
    indexA = this->model->index(0, 1, rootIndex);
    model->setData(indexA, "APrime", Qt::DisplayRole);
    indexA = this->model->index(0, 2, rootIndex);
    model->setData(indexA, "ABeta", Qt::DisplayRole);

    model->insertRow(1, rootIndex);
    indexB = this->model->index(1, 0, rootIndex);
    model->setData(indexB, "B", Qt::DisplayRole);

    model->insertRow(0, indexB);
    indexBChild = model->index(0, 0, indexB);
    rc = model->setData(indexBChild, "BChild", Qt::DisplayRole);
    if ( rc == false )
    {
        // This returns an error because the QStandardItemModel doesn't allow creating children for any
        // index except the root index.
        QMessageBox msg(QMessageBox::Warning, "Failure is an option", "Could not set child of second row");
        msg.exec();
    }
    */

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
