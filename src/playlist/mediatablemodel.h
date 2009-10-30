#ifndef MEDIATABLEMODEL_H
#define MEDIATABLEMODEL_H
#include <xmmsclient/xmmsclient++.h>
#include "../core/coreplaylist.h"
#include <QAbstractItemModel>
#include <QList>
 #include <QStringList>

 class QMimeData;



/**
 * This is a model of the core, that is, MediaTableModel is an interface to
 *the core, because, due to the core's design it doesn't implement a columns
 *model, it only knows about rows of metada ( more explanation required here )
 */
class MediaTableModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    MediaTableModel(QObject *parent = 0, CorePlaylist *core = 0);

    void setPlaylistName( QString name );
    void setCore ( CorePlaylist *core );

    QModelIndex index ( int row, int column,
                        const QModelIndex & parent = QModelIndex() ) const;
    QModelIndex parent ( const QModelIndex & index ) const;
    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index,
                    int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole ) const;
    void sort( int column, Qt::SortOrder order = Qt::AscendingOrder );

    //Drag and drop
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                       int row, int column, const QModelIndex &parent);
     QMimeData *mimeData(const QModelIndexList &indexes) const;
     QStringList mimeTypes() const;
     Qt::DropActions supportedDropActions() const;

private:
    QList < QString > keys;
    QList < QString > headers;
    CorePlaylist *core;

public slots:
    void updatePosition ( int pos );
    void updateData ( int pos, QString key );

private slots:
    void plRowsAboutToBeInserted( int first, int last );
    void plRowsInserted();
    void plRowsAboutToBeRemoved( int first, int last );
    void plRowsRemoved();

};

#endif // MEDIATABLEMODEL_H
