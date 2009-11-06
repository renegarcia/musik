#include "playlistwindow.h"
#include "qxmms2.h"
#include <QDockWidget>
#include <KApplication>
#include <KAction>
#include <KToggleAction>
#include <KLocale>
#include <KActionCollection>
#include <KStandardAction>
#include <QToolBox>


PlaylistWindow::PlaylistWindow( QWidget *parent, Xmms::Client *xmms )
        : KXmlGuiWindow( parent )
{
    playlist = new PlaylistWidget( this, xmms );
    this->setCentralWidget( playlist );

    plman = new PlaylistManagerWidget( this, xmms );
    connect( plman, SIGNAL(playlistSelected(QString)),
             playlist, SLOT(viewPlaylist(QString)) );

    dplman = new DPlaylistManagerWidget( this, xmms );
    connect( dplman, SIGNAL(dPlaylistSelected(QString)),
             playlist, SLOT(viewDynamicPlaylist(QString)) );

    QToolBox *toolbox = new QToolBox( this );
    toolbox->addItem( plman, "Playlists" );
    toolbox->addItem( dplman, "Dynamic playlists" );

    dock = new QDockWidget( tr("Playlists"), this );
    dock->setObjectName( "playlistmanager" );
    dock->setWidget( toolbox );
    addDockWidget(Qt::LeftDockWidgetArea, dock );

    //QDockWidget *dDock = new QDockWidget( tr("Dynamic playlists"), this );
    //dDock->setObjectName( "dplaylistmanager" );
    //dDock->setWidget( dplman );
    //addDockWidget( Qt::LeftDockWidgetArea, dDock );



    QXmms2 *qxmms2 = new QXmms2( this, xmms ); //Ugly!. We must change this

    connect( playlist, SIGNAL(play()), qxmms2, SLOT(play()));

    setupActions();
}


void
PlaylistWindow::setupActions()
{
  KAction* addMediaAction = new KAction(this);
  addMediaAction->setText( i18n("Add") );
  addMediaAction->setToolTip( i18n("Add media to the playlist") );
  addMediaAction->setIcon( KIcon("document-open") );
  actionCollection()->addAction( "addmedia", addMediaAction );
  connect(addMediaAction, SIGNAL(triggered()),
          playlist, SLOT(addMedia()));

  KAction* removeAction = new KAction(this);
  removeAction->setText( i18n("Remove selection") );
  removeAction->setToolTip( i18n("Remove selection from playlist") );
  removeAction->setIcon( KIcon("edit-delete") );
  removeAction->setShortcut( Qt::Key_Delete );
  actionCollection()->addAction( "remove", removeAction );
  connect(removeAction, SIGNAL(triggered(bool)),
          playlist, SLOT(removeSelection()));

  KToggleAction* plmanAction = new KToggleAction(this);
  plmanAction->setText( i18n("Playlist manager") );
  plmanAction->setToolTip( i18n("Show playlist manager") );
  plmanAction->setIcon( KIcon( "document-new" ) );
  plmanAction->setShortcut( Qt::CTRL + Qt::Key_M );
  actionCollection()->addAction( "plmanshow", plmanAction );
  connect( plmanAction, SIGNAL(toggled(bool)),
            dock, SLOT(setShown(bool)));
  connect(dock, SIGNAL(visibilityChanged(bool)),
          plmanAction, SLOT(setChecked(bool)));

  KToggleAction *findAction = new KToggleAction(this);
  findAction->setText( i18n("Find") );
  findAction->setIcon( KIcon("edit-find") );
  findAction->setShortcut( Qt::Key_J );
  actionCollection()->addAction( "find", findAction );
  connect( findAction, SIGNAL(toggled(bool)),
           playlist, SLOT(setFilterVisible(bool)));

  KStandardAction::quit(kapp, SLOT(quit()),
                        actionCollection());

  setupGUI();
}
