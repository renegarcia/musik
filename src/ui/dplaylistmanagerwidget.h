#ifndef DPLAYLISTMANAGERWIDGET_H
#define DPLAYLISTMANAGERWIDGET_H
#include <xmmsclient/xmmsclient++.h>

#include <KListWidget>
class QListWidgetItem;

class DPlaylistManagerWidget : public KListWidget
{
        Q_OBJECT

public:
    DPlaylistManagerWidget( QWidget *parent = 0, Xmms::Client *xmms = 0 );

public slots:
    //void newPlaylist ();
    //void remove ( const QString &name );
    void entryExecuted( QListWidgetItem *index );

signals:
    void dPlaylistSelected( const QString &name );

private:
    QListWidgetItem *selectedPlaylist;

    Xmms::Client *xmms;
    std::string activePlaylist;

    void gotConnection ( bool ok );
    bool collections_list_cb ( const Xmms::List< std::string > &list );
    bool collection_changed_cb( const Xmms::Dict &info );
    bool current_active_cb ( const std::string &activePlaylist );
    bool collection_get_cb ( const Xmms::Coll::Coll &collection );

};

#endif // DPLAYLISTMANAGERWIDGET_H
