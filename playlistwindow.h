#ifndef PLAYLISTWINDOW_H
#define PLAYLISTWINDOW_H
#include <xmmsclient/xmmsclient++.h>
#include <playlistmanagerwidget.h>
#include <playlistwidget.h>
#include <KXmlGuiWindow>


class QDockWidget;

class PlaylistWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    PlaylistWindow( QWidget *parent = 0, Xmms::Client *xmms = 0 );

private:
    PlaylistWidget *playlist;
    PlaylistManagerWidget *plman;
    QDockWidget *dock;
    void setupActions();
};

#endif // PLAYLISTWINDOW_H
