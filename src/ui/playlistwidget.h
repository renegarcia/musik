#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H
#include <xmmsclient/xmmsclient++.h>
#include "playlistmanagerwidget.h"
#include "coreplaylist.h"
#include "qxmms2.h"
#include "mediatablemodel.h"
#include <QWidget>
#include <QAbstractItemModel>
#include <QTreeView>
#include <KLineEdit>
#include "sortfilterproxymodel.h"

class QLabel;

class PlaylistWidget : public QWidget
{
Q_OBJECT

public:
    PlaylistWidget( QWidget *parent = 0, Xmms::Client *xmms = 0 );

    void setCore( CorePlaylist *core );

public slots:
    void removeSelection();
    void viewPlaylist( const QString &name );
    void viewDynamicPlaylist();
    void setFilterVisible( bool ok );
    void addMedia();
    void shuffle();

signals:
    void play();

private:
    //PlaylistManagerWidget *plman;
    QLabel *jumpLabel;
    QTreeView *view;
    KLineEdit *filter;
    SortFilterProxyModel *proxyModel;
    CorePlaylist *core;
    MediaTableModel *model;

private slots:
    void play( const QModelIndex index );

};

#endif // PLAYLISTWIDGET_H
