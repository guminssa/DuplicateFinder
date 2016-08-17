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
#include <exception>

#define DEFAULT_TOP_LEVEL_DIR_COUNT 5

class DataItem
{
public:
    DataItem(const QString &filePath, DataItem *parent);
    //bool isRecursive();
    QString &getHash();
    const QStringList &getSimilarDirs(); // Other dirs that contain duplicates of the files in this item
    bool addSimilarDir(DataItem *otherItem);
    QStringList &listDuplicates(const QString &path); // List all files in path that are duplicates of this item's files
    bool markDuplicate(DataItem *otherItem);
    bool setModelIndex(const QModelIndex &index);
    bool addChildItem(DataItem *child);


private:
    DataItem *parentItem;
    QString path;
    //QString parentDirname;
    //QModelIndex modelIndex;
    QFileInfo *fileInfo;

    // Only populated if this item is a directory
    QVector<DataItem *> *fileChildren, *dirChildren;
    QStringList similarDirs;
    QDir *dirInfo; // Only populated for directories, not files
    bool isRecursive;

    // Only populated if this item is a file
    QString hash;  // Only populated for files, not directories

    // Only takes action when item is a file
    bool calculateHash();

    // Internal calculations
    int numDirChildren();
    int numFileChildren();


    friend class DataModel;
};

#endif // DATAITEM_H