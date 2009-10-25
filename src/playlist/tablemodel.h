#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include <xmmsclient/xmmsclient++.h>
#include "../core/coreplaylist.h"
#include <QAbstractTableModel>
#include <QList>


class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject *parent = 0, Xmms::Client *xmms = 0);

    void setPlaylistName( QString name );

    int rowCount ( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex & index,
                    int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole ) const;

private:
    QList < QString > keys;
    QList < QString > headers;
    Xmms::Client *xmms;
    CorePlaylist *core;

public slots:
    void insertRow ( int pos );
};

#endif // TABLEMODEL_H
