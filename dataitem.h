#ifndef DATAITEM_H
#define DATAITEM_H
#include <QList>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QByteArray>
#include <QException>
#include <QDebug>
#include <QModelIndex>
#include <QStringList>
#include <exception>

#define DEFAULT_TOP_LEVEL_DIR_COUNT 3

class DataItem
{
public:
    // Constructors/destructors
    DataItem(const QString &filePath, DataItem *parent);
    ~DataItem();


    // Qt Model interface
    bool setModelIndex(const QModelIndex &index);


    // Data manipulation
    QString &getHash();
    const QList<DataItem *> *getSimilarDirs(); // Other dirs that contain duplicates of the files in this item
    bool addSimilarDir(DataItem *otherItem);
    const QList<DataItem *> &listDuplicates(); // List all files in path that are duplicates of this item's files
    bool markDuplicate(DataItem *otherItem);
    bool addChildItem(DataItem *child);
    bool removeFromParent();
    bool removeFromDuplicates(bool removeList);



private:

    // Qt Model interface
    QModelIndex modelIndex;


    // Data manipulation

    DataItem *parentItem;
    QString path;
    QFileInfo *fileInfo;


    // Only populated if this item is a directory
    QVector<DataItem *> *fileChildren, *dirChildren;
    QList<DataItem *> *similarDirs;
    QDir *dirInfo;
    bool isRecursive;

    // Only populated if this item is a file
    QString hash;  // Only populated for files, not directories
    QList<DataItem *> *duplicateFiles;

    // Only takes action when item is a file
    bool calculateHash();

    // Internal calculations
    int numDirChildren();
    int numFileChildren();


    friend class DataModel;
};

#endif // DATAITEM_H
