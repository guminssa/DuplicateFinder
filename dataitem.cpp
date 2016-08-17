#include "dataitem.h"

DataItem::DataItem(const QString &filePath, DataItem *parent=nullptr)
{
    qDebug() << "DataItem constructor for path " + filePath + ".";
    parentItem = parent;

    path = filePath;

    // The item with a parent set to nullptr is the root item, which doesn't actually map
    // to a file or directory
    if ( parentItem != nullptr )
    {
        qDebug() << "This item (" + filePath + ") has a parent";
        fileInfo = new QFileInfo(path);
        if ( !fileInfo->exists() )
        {
            throw new std::out_of_range(std::string("Path does not exist"));
            path = "";
            return;
        }

        qDebug() << "Got FileInfo";
        //parentDirname = QDir::dirName(path);


        if ( fileInfo->isDir() )
        {
            qDebug() << "This is a directory";
            dirInfo = new QDir(path);
            dirChildren = new QVector<DataItem *>(numDirChildren(), nullptr);
            fileChildren = new QVector<DataItem *>(numFileChildren(), nullptr);
        }
        else
        {
            qDebug() << "This is a file";
            if ( parentItem == nullptr )
            { throw new std::range_error(std::string("Root object must be a directory, not a file")); }
            calculateHash();
            dirChildren = nullptr;
            fileChildren = nullptr;
            dirInfo = nullptr;
        }

        qDebug() << "Adding this as a child item";
        parentItem->addChildItem(this);
        qDebug() << "Done adding this as a child item";
    }
    else
    {
        qDebug() << "This item is the root";
        // We don't know how many top-level directories the user is going to want to examine, so
        // just reserve space for several
        dirChildren = new QVector<DataItem *>(DEFAULT_TOP_LEVEL_DIR_COUNT, nullptr);
        fileChildren = nullptr;
        fileInfo = nullptr;
        dirInfo = nullptr;
    }

    qDebug() << "Created new DataItem for path " + path + ".";
}


bool DataItem::addChildItem(DataItem *child)
{
    if ( fileInfo == nullptr || !fileInfo->isDir() || child == nullptr || child->fileInfo == nullptr )
        { return false; }

    if ( child->fileInfo->isDir() && !dirChildren->contains(child) )
        {   dirChildren->append(this);    }
    else if ( !fileChildren->contains(child))
        {   fileChildren->append(this);   }
    else
        {   return false;   }

    child->parentItem = this;
    return true;  // Child item was added to either the dirChildren or the fileChildren list
}


bool DataItem::calculateHash()
{
    QFile file(path);
    QByteArray data;
    if ( fileInfo != nullptr && fileInfo->isFile() && file.open(QIODevice::ReadOnly) )
    {
        data = file.readAll();
        hash = QString(QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex());
        return true;
    }
    else
    {
        return false;
    }
}

QString &DataItem::getHash()
{
    return hash;
}

#if 0
bool DataItem::setModelIndex(const QModelIndex &index)
{
    modelIndex = index;
    return true;
}
#endif


bool DataItem::addSimilarDir(DataItem *otherItem)
{
    if ( fileInfo->isDir() && otherItem->fileInfo->isDir() )
    {
        // Set the dirs to point to each other
        if ( !similarDirs.contains(otherItem->path, Qt::CaseInsensitive) )
        {
            similarDirs.append(otherItem->path);
        }
        if ( !otherItem->similarDirs.contains(path, Qt::CaseInsensitive) )
        {
            otherItem->similarDirs.append(path);
        }
    }
    else if ( fileInfo->isFile() && otherItem->fileInfo->isFile() )
    {
        // Update the parent directories instead
        if ( !parentItem->similarDirs.contains(otherItem->parentItem->path, Qt::CaseInsensitive) )
        {
            parentItem->similarDirs.append(otherItem->parentItem->path);
        }
        if ( !otherItem->similarDirs.contains(path, Qt::CaseInsensitive) )
        {
            otherItem->similarDirs.append(path);
        }
    }
    else
    {
        // This item and the otherItem are of mixed types, return error
        return false;
    }
    return true;
}

const QStringList &DataItem::getSimilarDirs()
{
    return similarDirs;
}

bool DataItem::markDuplicate(DataItem *otherItem)
{
    if ( otherItem == nullptr )
        return false;


    return false; // TODO
}

int DataItem::numDirChildren()
{
    if ( fileInfo->isFile() ) { return 0; } // Files can't have any children
    return dirInfo->entryList(QDir::Dirs).size();
}

int DataItem::numFileChildren()
{
    if ( fileInfo->isFile() ) { return 0; } // Files can't have any children
    return dirInfo->entryList(QDir::Files).size();
}

#if 0
QStringList &listDuplicates(const QString &path)
{
    //TODO
}
#endif
