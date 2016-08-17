#include "datamodel.h"

DataModel::DataModel(QObject *parent) : QAbstractTableModel(parent)
{
    rootItem = new DataItem(QString(""), nullptr);
    // TODO: Create model index
}


DataModel::DataModel(QString baseDir, bool recurse, QObject *parent) : QAbstractTableModel(parent)
{

    rootItem = new DataItem(QString(""), nullptr);
    // TODO: Create model index

    addPath(baseDir, recurse);
}


DataModel::~DataModel()
{
    // TODO: Create model index
}


bool DataModel::addPath(QString &baseDir, bool recurse, DataItem *parent)
{
    QFileInfo file(baseDir);

    if ( !file.exists() || !file.isDir() )
    {
        qDebug() << "Path " + baseDir + " does not exist or is not a directory";
        return false;
    }

    // Check if this path has already been added


    if ( pathAlreadyAdded(baseDir) )
        return true;


    // Create new DataItem to represent the specified baseDir
    DataItem *newItem = createDataItem(baseDir, parent);


    if ( recurse )
    {

        // Create a DataItem for each file in the new directory
        QStringList childFiles = newItem->dirInfo->entryList(QDir::Files);
        qDebug() << newItem->path << " has " << childFiles.count() << " child files";
        for (QList<QString>::iterator it = childFiles.begin(); it != childFiles.end(); it++)
        {
            QString newPath = newItem->path + QDir::separator() + (*it); // Prepend the dirname
            createDataItem(newPath, newItem); // Create a DataItem for the child file
        }
    }

    newItem->isRecursive = recurse;
    return true;
}


DataItem *DataModel::createDataItem(QString &path, DataItem *parent)
{
    if ( parent == nullptr ) { parent = rootItem; }

    DataItem *newItem = new DataItem(path, parent);

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
                qDebug() << "New file " << newItem->path << " matches existing file " << (*matches)->path;
                qDebug() << "New hash=" << newItem->getHash() << " existing hash=" << (*matches)->getHash();
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
bool DataModel::pathAlreadyAdded(const QString &path)
{
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


int DataModel::rowCount(const QModelIndex &/*parent*/) const
{
#if 0
    DataItem *item = DataModel::itemFromIndex(parent);

    if ( item == 0 )
    {
        return 0;
    }

    return item->fileChildren->size() + item->dirChildren->size(); // TODO: This is private
#endif
    return 0;
}


int DataModel::columnCount(const QModelIndex &/*parent*/) const
{
#if 0
    DataItem *item = DataModel::itemFromIndex(parent);

    if ( item == 0 )
    {
        return 0;
    }

    return 1;
#endif
    return 1;
}


#if 0
static DataItem *DataModel::itemFromIndex(const QModelIndex &index)
{
    if ( index == QModelIndex() )
        return nullptr;
    return (DataItem *) index.internalId();
}
#endif


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


#if 0
QVariant DataModel::data(const QModelIndex & index, int role) const
{
    // Apparently, the index parameter is always marked as invalid
    DataItem *item = itemFromIndex(parent);
    switch (role)
    {
        case (Qt::DisplayRole):
            return item->path; // TODO: This is private
            break;
        default:
            return QVariant(); // No data
    }

}



QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE
{
    return QModelIndex(); // TODO
}
#endif



QModelIndex DataModel::index(int /*row*/, int /*column*/,
                  QModelIndex const&) const
{
    return QModelIndex(); // TODO
}



QModelIndex DataModel::parent(QModelIndex const&) const
{
    return QModelIndex(); // TODO
}

QVariant DataModel::data(QModelIndex const& /*index*/, int /*role = Qt::DisplayRole*/) const
{
        return QVariant(); // TODO
}
