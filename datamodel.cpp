#include "datamodel.h"

//******************************************************************
//
// Constructors/destructors
//
//******************************************************************

DataModel::DataModel(QObject *parent) : QAbstractItemModel(parent)
{
    rootItem = new DataItem(QString("Root"), nullptr);
    rootItem->modelIndex = QModelIndex();
}


DataModel::DataModel(QString baseDir, bool recurse, QObject *parent) : QAbstractItemModel(parent)
{

    rootItem = new DataItem(QString("Root"), nullptr);
    rootItem->modelIndex = QModelIndex();

    addPath(baseDir, recurse, rootItem);
}


DataModel::~DataModel()
{

}



//******************************************************************
//
// Qt Model interface
//
//******************************************************************

int DataModel::rowCount(const QModelIndex &parent) const
{
    DataItem *item;
    int rc=0;


    if ( !parent.isValid() )
    {
        item = rootItem;
    }
    else
    {
        item = (DataItem *) parent.internalPointer();
        //qDebug() << " Item = " << item;
        if (item->fileInfo != nullptr && item->fileInfo->isFile())
        {
            qDebug() << "rowCount(" << parent << ") returning " << rc;
            return rc;
        }
    }

    if ( item->fileChildren != nullptr )
    {
        rc += item->fileChildren->size();
    }
    if ( item->dirChildren != nullptr)
    {
        rc += item->dirChildren->size();
    }

    if ( parent.isValid() )
        qDebug() << "rowCount(" << parent << ") returning " << rc;
    return rc;
}


int DataModel::columnCount(const QModelIndex &parent) const
{
    /*
    if ( parent.isValid() )
        qDebug() << " Item = " << parent.internalPointer();
    */
    if ( parent.isValid() )
        qDebug() << "columnCount(" << parent << ") returning 1";
    return 1;
}



/*
bool DataModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    // TODO
    if ( index != QModelIndex() || role != Qt::DisplayRole || value == "Invalid" )
        return false;
    return false;
}


Qt::ItemFlags DataModel::flags(const QModelIndex & index) const
{
    // TODO
    if ( index == QModelIndex() )
        return 0;
    else
        return 0;
}
*/


// Returns the Model Index for the specified data item.  This function is called by
// the View object to generate a Model Index.
//
// The view will always call this function first, before ever attempting to access it
// with the data() function.
QModelIndex DataModel::index(int row, int column,
                  const QModelIndex &parent) const
{
    DataItem *parentItem, *requestedItem;
    int dirCount=0;
    int fileCount=0;
    int childIndex = 0;


    qDebug() << "index(" << row << ", " << column << ", " << parent << ")";
    if ( !parent.isValid() )
    {
        //qDebug() << "ParentIndex is rootItem";
        parentItem = rootItem;
    }
    else
    {
        /*
        qDebug() << "ParentIndex is NOT rootItem, pointer=" << parent.internalPointer()
                 << ", id=" << parent.internalId();
        */
        parentItem = (DataItem *) parent.internalPointer();
        //qDebug() << "ParentIndex = " << parentItem->path;
    }

    if ( parentItem->dirChildren != nullptr )
        dirCount = parentItem->dirChildren->count();
    if ( parentItem->fileChildren != nullptr )
        fileCount = parentItem->fileChildren->count();

    // The model represents the child directories as the first set of rows,
    // followed by the child files

    childIndex = row;
    if ( childIndex < dirCount && parentItem->dirChildren != nullptr )
    {
        // Get the DataItem that represents the child directory
        requestedItem = parentItem->dirChildren->value(childIndex);
    }
    else
    {
        childIndex -= dirCount;
        if ( childIndex < fileCount && parentItem->fileChildren != nullptr )
        {
            requestedItem = parentItem->fileChildren->value(childIndex);
        }
        else
        {
            qDebug() << "Item does not exist, return invalid index (remaining rows="
                     << childIndex << ")";
            return QModelIndex();
        }
    }

    if ( requestedItem == nullptr )
    {
        //qDebug() << "requestedItem = null";
        return QModelIndex();
    }
    /*
    else
    {
        qDebug() << "requestedItem is " << requestedItem->path;
    }
    */

    if ( !requestedItem->modelIndex.isValid() )
    {
        requestedItem->modelIndex = createIndex(row,column, (void *) requestedItem);
    }
    if ( requestedItem->modelIndex.row() != row )
    {
        QMessageBox msg(QMessageBox::NoIcon, "Alert",
            "createIndex mismatch for " + requestedItem->path + ": row " + requestedItem->modelIndex.row() + "does not match requested row " + row);
        msg.exec();
        requestedItem->modelIndex = createIndex(row,column, (void *) requestedItem);
    }
    // For some reason, createIndex is generating invalid indicies
    // (i.e. the row numbers are negative)
    qDebug() << " Returning new QModelIndex " << requestedItem->modelIndex <<
                " for item " << requestedItem << " " << requestedItem->path;
    /*
    qDebug() << " internalPointer=" << requestedItem->modelIndex.internalPointer() <<
                " internalID=" << requestedItem->modelIndex.internalId();
    */
    return requestedItem->modelIndex;
}

QVariant DataModel::headerData(int section, Qt::Orientation /*orientation*/, int role) const
{
    switch (role)
    {
        case Qt::DisplayRole:
            if ( section == 0 )
                return QVariant("Header");
            else
                return QVariant();
            break;
        default:
            return QVariant();
    }
}

QModelIndex DataModel::parent(const QModelIndex &child) const
{
    DataItem *childItem, *parentItem;

    //qDebug() << "childItem";
    if ( !child.isValid() ) // The root item has no parent
        return QModelIndex();

    childItem = (DataItem *) child.internalPointer();
    if ( childItem == nullptr) // This should never happen.
        return QModelIndex();

    parentItem = childItem->parentItem;
    if ( parentItem == nullptr) // This should never happen.
        return QModelIndex();

    return parentItem->modelIndex;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
    // Qt::DisplayRole
    DataItem *item;
    if ( role == Qt::DisplayRole || role == Qt::ToolTipRole )
        qDebug() << "data(" << index << ") start, role=" << role;
    if ( !index.isValid() )
        item = rootItem;
    else
        item = (DataItem *) index.internalPointer();

    switch (role)
    {
        case Qt::DisplayRole:
        //case Qt::EditRole:
        case Qt::ToolTipRole:
        //case Qt::StatusTipRole:
        //case Qt::WhatsThisRole:
            if ( item == rootItem )
            {
                //qDebug() << "DisplayRole requested for rootItem.  Returning Root";
                return QVariant("Root"); // Nothing to display
            }
            else
            {
                if ( role == Qt::DisplayRole || role == Qt::ToolTipRole )
                    qDebug() << "  Returning " << item->path;
                return QVariant(item->path);
            }
        break;
        /*
        case Qt::DecorationRole:
            if ( item->fileInfo->isDir() )
                return QVariant(Qt::QIcon)
        */
        default:
            return QVariant();
    }
    return QVariant(); // TODO
}











//******************************************************************
//
// Data manipulation
//
//******************************************************************



bool DataModel::addPath(QString &baseDir, bool recurse, DataItem *parent)
{
    QFileInfo file(baseDir);

    if ( !file.exists() || !file.isDir() )
    {
        qDebug() << "Path " + baseDir + " does not exist or is not a directory";
        return false;
    }

    if ( pathAlreadyAdded(baseDir) )
    {
        qDebug() << baseDir << " was previously added.  Skipping.";
        return true;
    }


    // Create new DataItem to represent the specified baseDir
    DataItem *newItem = createDataItem(baseDir, parent);

    // Create a DataItem for each file in the new directory
    QStringList childFiles = newItem->dirInfo->entryList(QDir::Files);
    for (QList<QString>::iterator it = childFiles.begin(); it != childFiles.end(); it++)
    {
        QString newPath = newItem->path + QDir::separator() + (*it); // Prepend the dirname
        createDataItem(newPath, newItem); // Create a DataItem for the child file
    }

    if ( recurse ) // Recurse into subdirectories
    {
        QStringList childDirs = newItem->dirInfo->entryList(QDir::Dirs);
        for (QList<QString>::iterator it = childDirs.begin(); it != childDirs.end(); it++)
        {
            if ( (*it)!= "." && (*it) != ".." )
            {
                QString subPath = newItem->path + QDir::separator() + (*it); // Prepend the dirname
                addPath(subPath, true, newItem);
            }
        }
    }

    newItem->isRecursive = recurse;
    return true;
}


DataItem *DataModel::createDataItem(QString &path, DataItem *parent)
{
    if ( parent == nullptr ) { parent = rootItem; }
    /*
    if ( parent == rootItem ) { qDebug() << "Parent is rootItem"; }
    else { qDebug() << "Parent is not rootItem"; }
    */

    DataItem *newItem = new DataItem(path, parent);
    //newItem->modelIndex = createIndex(0, 0, newItem);

    // TODO: Create a ModelIndex

    // If this is a file (not a dir), add its hash to the global hash list.  If
    // there are already other files with the same hash, update this new item and
    // any matches to point to each other as duplicates
    if ( newItem->fileInfo->isFile() )
    {
        QString hash = newItem->getHash();
        if ( hashes.contains(hash) ) // If another DataItem has the same hash...
        {
            newItem->duplicateFiles = new QList<DataItem *>; // Create a list to store the duplicates

            QMap<QString, DataItem *>::iterator matches = hashes.find(hash);
            while ( matches != hashes.end() && (*matches)->getHash() == hash )
            {
                // For each DataItem in the master hash list with this hash, update them to
                // point to each other.
                newItem->markDuplicate(*matches);
                matches++;
            }
        }
        hashes.insert(newItem->getHash(), newItem);
    }

    return newItem;
}


// Returns true if the requested path has previously been added to the data model
// TODO: This is a very simple implemenation that will cause errors if there are
// directories higher in the hierarchy that have the same name but one has additional
// characters.  For example, /home/steve and /home/steveG would cause problems.
bool DataModel::pathAlreadyAdded(const QString &/*path*/)
{
#if 0
    int count = 1;
    if ( rootItem->dirChildren == nullptr )
    {
        return false;
    }


    for ( QVector<DataItem *>::iterator it = rootItem->dirChildren->begin(); it != rootItem->dirChildren->end(); it++ )
    {
        count++;
        DataItem *item = (DataItem *) *it;
        if ( item == nullptr ) continue;
        QString &existingPath = item->path;
        if ( path == existingPath ||
                (item->isRecursive && path.startsWith(existingPath)) )
        {
            qDebug() << path << " was previously added";
            return true;
        }
    }
#endif
    return false;
}


void DataModel::debugDuplicates()
{
    QMultiMap<QString, DataItem *>::iterator baseIterator;
    QList<DataItem *>::iterator childIterator;
    DataItem *item;

    // Note: Since the hashes map only contains files, no need to check if each entry is
    // a directory or a file.
    for ( baseIterator = hashes.begin(); baseIterator != hashes.end(); baseIterator++ )
    {
        item = (DataItem *) (*baseIterator);
        if ( item->duplicateFiles != nullptr && item->duplicateFiles->size() > 0 )
        {
            qDebug() << item->path << " has duplicate files:";
            for ( childIterator = item->duplicateFiles->begin(); childIterator != item->duplicateFiles->end(); childIterator++ )
            {
                qDebug() << "  " << (*childIterator)->path;
            }
        }
    }
}

void DataModel::dumpFiles()
{
    DataItem *item = rootItem;
    DataItem *childItem;
    //QVector<DataItem *> vector;
    QVector<DataItem *>::iterator it;

    qDebug() << "dumpFiles start";
    if ( item->dirChildren != nullptr )
    {
        qDebug() << "rootItem has " << item->dirChildren->count() << " child dirs";
        //vector = *(item->dirChildren);
        for ( it = item->dirChildren->begin(); it != item->dirChildren->end(); it++)
        {
            childItem = (DataItem *) (*it);
            if ( childItem != nullptr )
                qDebug() << "Child dir: " << childItem->path;
            else
                qDebug() << "rootItem child dir is null";
        }
    }

    if ( item->fileChildren != nullptr )
    {
        qDebug() << "rootItem has " << item->fileChildren->count() << " child files";
        //vector = *(item->dirChildren);
        for ( it = item->fileChildren->begin(); it != item->fileChildren->end(); it++)
        {
            childItem = (DataItem *) (*it);
            if ( childItem != nullptr )
                qDebug() << childItem->path;
        }
    }
    else
    {
        qDebug() << "rootItem has no file children";
    }
    qDebug() << "dumpFiles end";
}
