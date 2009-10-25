#include "tablemodel.h"

TableModel::TableModel( QObject *parent, Xmms::Client *xmms )
        : QAbstractTableModel( parent )
{
    this->xmms = xmms;
    keys << "tracknr" << "title" << "album" << "artist" << "duration"
            << "timesplayed";
    headers << "Track" << "Title" << "Album" << "Artist" << "Duration"
            << "Times played";

    core = new CorePlaylist( this, xmms );
    connect ( core, SIGNAL( rowInserted( int ) ), this,
              SLOT( insertRow( int ) ) );

    QList < QString > coreKeys( keys );
    coreKeys << "picture_front";
    core->setKeys( coreKeys );
}

int
TableModel::rowCount( const QModelIndex &parent ) const
{
    return core->rowCount();
}

int
TableModel::columnCount( const QModelIndex &parent ) const
{
    return headers.size();
}

QVariant
TableModel::headerData ( int section, Qt::Orientation orientation,
                         int role) const
{
    if ( (orientation == Qt::Horizontal) && (role == Qt::DisplayRole) )
        return QVariant ( headers[section] );

    return QVariant();
}

QVariant
TableModel::data ( const QModelIndex & index,
                    int role ) const
{
    if ( !index.isValid() )
        return QVariant ();

    if ( role != Qt::DisplayRole )
        return QVariant ();

    int row = index.row();
    int col = index.column();

    QString key = keys[ col ];
    return core->value( row, key);
}

void
TableModel::insertRow( int pos )
{
    beginInsertRows ( QModelIndex(), pos, pos );
    endInsertRows ();
}
