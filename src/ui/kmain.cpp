#include <xmmsclient/xmmsclient++.h>
#include "xmmsqt4.h"
#include "mediatablemodel.h"
#include "playlistwidget.h"
#include "coreplaylist.h"
#include "qxmms2.h"
#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>
#include <KLocale>
#include <QShortcut> //UUUUGGGGLLLYYY


int main(int argc, char *argv[])
{
    KAboutData aboutData ( "musik",
                           0,
                           ki18n( "Musik" ),
                           "0.0",
                           ki18n( "xmms2 interface" ),
                           KAboutData::License_GPL_V3,
                           ki18n( "(C) 2009"),
                           ki18n( "blablabla"),
                           "http://wiki.xmms2.xmms.se",
                           "noname@nowhere.com");
    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication a;
    Xmms::Client *xmms = new Xmms::Client("musik");
    try
    {
        xmms->connect(std::getenv("XMMS_PATH"));
    }
    catch (Xmms::connection_error error)
    {
        qWarning("Could not connect to server");
        exit(1);
    }
    xmms->setMainloop(new XmmsQT4(xmms->getConnection()));

    CorePlaylist *core = new CorePlaylist( 0, xmms );
    QXmms2 *qxmms2 = new QXmms2( 0, xmms );

    PlaylistWidget *pl = new PlaylistWidget(0, core);//better!
    QShortcut *sc = new QShortcut( Qt::Key_Delete, pl );
    pl->connect( pl, SIGNAL(play()), qxmms2, SLOT(play()));
    pl->connect( sc, SIGNAL(activated()), pl, SLOT(removeSelection()));
    pl->show();
    return a.exec();
}
