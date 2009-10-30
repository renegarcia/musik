#ifndef PLAYLISTMANAGERWIDGET_H
#define PLAYLISTMANAGERWIDGET_H
#include <xmmsclient/xmmsclient++.h>
#include <list>
#include <KListWidget>

class QListWidgetItem;
class QModelIndex;

class PlaylistManagerWidget : public KListWidget
{
    Q_OBJECT

public:
    PlaylistManagerWidget( QWidget *parent = 0, Xmms::Client *xmms = 0 );

public slots:

    void newPlaylist ();
    void remove ( const QString &name );
    void entryExecuted( QListWidgetItem *index );

signals:
    void playlistSelected( const QString &name );

private:
    QListWidgetItem *selectedPlaylist;

    Xmms::Client *xmms;
    std::string activePlaylist;

    void gotConnection ( bool ok );
    bool playlists_list_cb ( const Xmms::List< std::string > &list );
    bool collection_changed_cb( const Xmms::Dict &info );
    bool current_active_cb ( const std::string &activePlaylist );


};

#endif // PLAYLISTMANAGERWIDGET_H
