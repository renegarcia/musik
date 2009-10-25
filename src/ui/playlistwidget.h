#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H
#include "coreplaylist.h"
#include "qxmms2.h"
#include "mediatablemodel.h"
#include <QWidget>
#include <QAbstractItemModel>
#include <QTreeView>
#include <KLineEdit>
#include "sortfilterproxymodel.h"

class PlaylistWidget : public QWidget
{
Q_OBJECT

public:
    PlaylistWidget( QWidget *parent = 0, CorePlaylist *core = 0 );

    void setCore( CorePlaylist *core );

public slots:
    void removeSelection();

signals:
    void play();

private:
    QTreeView *view;
    KLineEdit *filter;
    SortFilterProxyModel *proxyModel;
    CorePlaylist *core;
    MediaTableModel *model;

private slots:
    void play( const QModelIndex index );

};

#endif // PLAYLISTWIDGET_H
