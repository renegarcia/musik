#include "playlistwindow.h"
#include "qxmms2.h"
#include <QDockWidget>
#include <KApplication>
#include <KAction>
#include <KToggleAction>
#include <KLocale>
#include <KActionCollection>
#include <KStandardAction>


PlaylistWindow::PlaylistWindow( QWidget *parent, Xmms::Client *xmms )
        : KXmlGuiWindow( parent )
{
    playlist = new PlaylistWidget( this, xmms );
    plman = new PlaylistManagerWidget( this, xmms );

    this->setCentralWidget( playlist );

    dock = new QDockWidget( tr("Playlists"), this );
    dock->setObjectName( "playlistmanager" );
    dock->setWidget( plman );
    addDockWidget(Qt::LeftDockWidgetArea, dock );

    connect( plman, SIGNAL(playlistSelected(QString)),
             playlist, SLOT(viewPlaylist(QString)) );

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
