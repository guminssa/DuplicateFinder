#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include <QMessageBox>
#include "datamodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pathEdit_returnPressed();

    void on_addButton_clicked();

private:
    Ui::MainWindow *ui;
    //QStandardItemModel *model;
    DataModel *model;
};

#endif // MAINWINDOW_H
