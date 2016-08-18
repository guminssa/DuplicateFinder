#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QMultiMap>
#include <QDebug>
#include "dataitem.h"
#include <exception>

class DataModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    // Constructors/destructors
    DataModel(QObject *parent = nullptr);
    DataModel(QString baseDir, bool recurse=false, QObject *parent = nullptr);
    ~DataModel();


    // Qt Model interface
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(QModelIndex const& index, int /*role = Qt::DisplayRole*/) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent) const;

    QModelIndex parent(QModelIndex const&) const;
    static DataItem *itemFromIndex(const QModelIndex &index);
    // For editing
    //bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    //Qt::ItemFlags flags(const QModelIndex & index) const;
    //void byIndexValid(QString pos);


    // Data manipulation
    bool addPath(QString &baseDir, bool recurse, DataItem *parent=nullptr);
    bool pathAlreadyAdded(const QString &path);
    void debugDuplicates(); // Display duplicate files





private:
    DataItem *rootItem;
    QMultiMap<QString, DataItem *> hashes;
    DataItem *createDataItem(QString &path, DataItem *parent=nullptr);
};

#endif // DATAMODEL_H
