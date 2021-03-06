#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QMessageBox>
#include <QDebug>
#include <QThread>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QMultiMap>
#include <QDebug>
#include <QTextStream>
#include "dataitem.h"
#include <exception>

class DataModel : public QAbstractItemModel
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

    QModelIndex parent(const QModelIndex &child) const;
    static DataItem *itemFromIndex(const QModelIndex &index);
    QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const;
    // For editing
    //bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    //Qt::ItemFlags flags(const QModelIndex & index) const;
    //void byIndexValid(QString pos);


    // Data manipulation
    bool addPath(const QString &baseDir, bool recurse, DataItem *parent);
    bool pathAlreadyAdded(const QString &);
    void debugDuplicates(); // Display duplicate files
    void dumpFiles();
    const QStringList &listDuplicates(const QModelIndex &modelIndex);





private:
    DataItem *rootItem;
    QMultiMap<QString, DataItem *> hashes;
    DataItem *createDataItem(const QString &path, DataItem *parent=nullptr);
    QStringList duplicateList;
};

#endif // DATAMODEL_H
