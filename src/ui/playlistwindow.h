#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H
#include <xmmsclient/xmmsclient++.h>
#include <playlistmanagerwidget.h>
#include <dplaylistmanagerwidget.h>
#include <playlistwidget.h>
#include <KXmlGuiWindow>


class QDockWidget;
class QXmms2;

class PlaylistWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    PlaylistWindow( QWidget *parent = 0, Xmms::Client *xmms = 0 );

private:
    QXmms2 *qxmms;
    PlaylistWidget *playlist;
    PlaylistManagerWidget *plman;
    DPlaylistManagerWidget *dplman;
    QDockWidget *dock;
    void setupActions();
};

#endif // PLAYLISTWINDOW_H
