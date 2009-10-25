#include <xmmsclient/xmmsclient++.h>
#include "xmmsqt4.h"
#include "mediatablemodel.h"
#include "playlistwidget.h"
#include <QStandardItemModel>
#include <QApplication>
#include <QTreeView>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
    PlaylistWidget *pl = new PlaylistWidget();
    //QTreeView *tv = new QTreeView( );
    //tv->setSelectionBehavior( QAbstractItemView::SelectRows );
    //tv->setAlternatingRowColors( true );
    //tv->setRootIsDecorated( false );
    MediaTableModel *model = new MediaTableModel( 0, xmms );
    pl->setModel( model );
    pl->show();
    //tv->setModel( model );
    //tv->sortByColumn( -1 );
    //tv->setSortingEnabled( true );
    //tv->show();
    return a.exec();
}
