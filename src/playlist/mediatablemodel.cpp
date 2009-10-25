#include "mediatablemodel.h"
#include <QFont>
#include <QMimeData>

QString ms2time(int milliseconds)
{
    QString fmtTime;
    int _time,
        time = milliseconds / 1000;

    _time = time / 3600;
    time = time % 3600;
    if (_time > 0)
        (_time < 10) ? fmtTime.append("0" + QString::number(_time) + " : ") : fmtTime.append(QString::number(_time) + " : ");
    _time = time / 60;
    time = time % 60;
    (_time < 10) ? fmtTime.append("0" + QString::number(_time) + " : ") : fmtTime.append(QString::number(_time) + " : ");
    (time < 10) ? fmtTime.append("0" + QString::number(time)) : fmtTime.append(QString::number(time));
    return fmtTime;
}


MediaTableModel::MediaTableModel( QObject *parent, CorePlaylist *core )
        : QAbstractItemModel( parent )
{
    this->core = core;
    keys << "tracknr" << "title" << "album" << "artist" << "duration"
            << "timesplayed";
    headers << "Track" << "Title" << "Album" << "Artist" << "Duration"
            << "Times played";

    connect ( core, SIGNAL( rowInserted( int ) ), this,
              SLOT( insertRow( int ) ) );

    connect ( core, SIGNAL(rowsRemoved(int, int )), this,
              SLOT( removeRows( int, int ) ) );

    connect ( core, SIGNAL(positionChanged(int)), this,
              SLOT( updatePosition( int ) ) );

    QList < QString > coreKeys( keys );
    coreKeys << "picture_front";
    core->setKeys( coreKeys );
}

void
MediaTableModel::setCore( CorePlaylist *core )
{
    this->core = core;
}

QModelIndex
MediaTableModel::index ( int row, int column,
                        const QModelIndex & parent ) const
{
    return createIndex( row, column );
}

QModelIndex
MediaTableModel::parent ( const QModelIndex & index ) const
{
    return QModelIndex();
}

int
MediaTableModel::rowCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return 0;

    return core->rowCount();
}

int
MediaTableModel::columnCount( const QModelIndex &parent ) const
{
    if ( parent.isValid() )
        return 0;

    return headers.size();
}

QVariant
MediaTableModel::headerData ( int section, Qt::Orientation orientation,
                         int role) const
{
    if ( (orientation == Qt::Horizontal) && (role == Qt::DisplayRole) )
        return QVariant ( headers[section] );

    return QVariant();
}

void
MediaTableModel::sort( int column, Qt::SortOrder order )
{
    if ( column == -1 )
        return;

    QString key = keys [ column ];

    if ( order == Qt::DescendingOrder )
        key = "-" + key;

    core->sort( key );

}

QVariant
MediaTableModel::data ( const QModelIndex & index,
                    int role ) const
{
    if ( !index.isValid() )
        return QVariant ();

    else if ( role == Qt::FontRole ){
        int row = index.row();
        QFont font;
        if ( row == core->currentPosition() ){
            font.setBold( true );
            return QVariant ( font );
        }
    }

    else if ( role == Qt::TextAlignmentRole ) {
        int col = index.column();
        QString key = keys [ col ];
        if ( ( key == "timesplayed" ) || ( key == "tracknr" )
            || (key == "duration"))
            return QVariant ( Qt::AlignCenter );
        else
            return QVariant ();
    }

    if ( (role != Qt::DisplayRole) && (role != Qt::ToolTipRole) )
        return QVariant ();

    int row = index.row();
    int col = index.column();

    QString key = keys[ col ];

    if ( key == "duration" ){
        int ms = core->value(row, key).toInt();
        QString time = ms2time( ms );
        return QVariant ( time );
    }

    return core->value( row, key);
}

void
MediaTableModel::insertRow( int pos )
{
    beginInsertRows ( QModelIndex(), pos, pos );
    endInsertRows ();
}

void
MediaTableModel::removeRows( int first, int last )
{
    beginRemoveRows( QModelIndex (), first, last );
    endRemoveRows ();
}

void
MediaTableModel::updatePosition( int pos )
{
    int colCount = keys.size();
    emit dataChanged( createIndex( pos, 0 ), createIndex( pos, colCount ));
}

//Drag and drop

Qt::ItemFlags
MediaTableModel::flags(const QModelIndex &index) const
 {
     if (index.isValid())
         return (Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

     return Qt::ItemIsDropEnabled;
 }

QStringList
MediaTableModel::mimeTypes() const
 {
     QStringList types;
     types << "application/x-xmms2mlibid" << "application/x-xmms2poslist" << "text/uri-list";
     return types;
 }

QMimeData *
MediaTableModel::mimeData(const QModelIndexList &indexes) const
{
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;

     QDataStream stream(&encodedData, QIODevice::WriteOnly);

     QList<int> list;
     foreach (QModelIndex index, indexes) {
         if (index.isValid() && index.column() == 0) {
             list.append( index.row() );
         }
     }

     stream << list;

     mimeData->setData("application/x-xmms2poslist", encodedData);
     return mimeData;
}

bool
MediaTableModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                int row, int column, const QModelIndex &parent)
 {
     if ( !data->hasFormat("application/x-xmms2poslist") )
         return false;

     if (action == Qt::IgnoreAction)
         return true;

     //int endRow;

     /*if (!parent.isValid()) {
         if (row < 0)
             endRow = core->rowCount();
         else
             endRow = qMin(row, pixmaps.size());
     } else
         endRow = parent.row();*/

    QByteArray encodedData = data->data("application/x-xmms2poslist");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QList<int> list;
    stream >> list;

    qSort( list );

    int newPos = 0;
    if ( !parent.isValid() ) {
        if ( row >= 0 ) {
            newPos = row;
        }
    }
    else {
        //initPos = parent.row() + 1;
        return false;
    }

    int counter = 0, oldPos = 0;
    foreach ( int pos, list ){
        oldPos = pos + counter;
        core->move( oldPos, newPos );
        if ( oldPos < newPos )
            counter--;
        else
            newPos++;
    }

    return true;
 }

Qt::DropActions
MediaTableModel::supportedDropActions() const
 {
     return Qt::CopyAction | Qt::MoveAction;
 }
