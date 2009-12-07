#include "playlistwindow.h"
#include "qxmms2.h"
#include <seekslider.h>
#include <QDockWidget>
#include <KApplication>
#include <KAction>
#include <KToggleAction>
#include <KLocale>
#include <KActionCollection>
#include <KStandardAction>
#include <KStatusBar>
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
             playlist, SLOT(viewDynamicPlaylist()) );

    QToolBox *toolbox = new QToolBox( this );
    toolbox->addItem( plman, "Playlists" );
    toolbox->addItem( dplman, "Dynamic playlists" );

    dock = new QDockWidget( this );
    dock->setObjectName( "playlistmanager" );
    dock->setWidget( toolbox );
    dock->setFeatures( QDockWidget::DockWidgetMovable
                       | QDockWidget::DockWidgetClosable );
    addDockWidget(Qt::LeftDockWidgetArea, dock );

    qxmms = new QXmms2( this, xmms ); //Ugly!. We must change this
    SeekSlider *slider = new SeekSlider( this );
    statusBar()->addPermanentWidget( slider );
    connect(qxmms, SIGNAL(trackDurationChanged(int)),
            slider, SLOT(setTrackDuration(int)));
    connect(qxmms, SIGNAL(trackPositionChanged(int)),
            slider, SLOT(setTrackPosition(int)));
    connect(slider, SIGNAL(seek(int)),
            qxmms, SLOT(seek(int)));

    connect( playlist, SIGNAL(play()), qxmms, SLOT(play()));

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

  KToggleAction *playAction = new KToggleAction( this );
  playAction->setText( i18n("Play") );
  playAction->setIcon( KIcon("media-playback-start") );
  playAction->setShortcut( Qt::Key_P );
  actionCollection()->addAction( "play", playAction );
  connect( playAction, SIGNAL( toggled(bool) ),
           qxmms, SLOT( playpause(bool)) );
  connect( qxmms, SIGNAL( serverIsPlaying(bool) ),
           playAction, SLOT( setChecked(bool) ));

  KAction *nextAction = new KAction( this );
  nextAction->setText( i18n("Next") );
  nextAction->setIcon( KIcon("media-skip-forward") );
  nextAction->setShortcut( Qt::Key_N );
  actionCollection()->addAction( "next", nextAction );
  connect( nextAction, SIGNAL(triggered()),
           qxmms, SLOT(next()));

  KAction *prevAction = new KAction( this );
  prevAction->setText( i18n("Previous") );
  prevAction->setIcon( KIcon("media-skip-backward") );
  prevAction->setShortcut( Qt::Key_B );
  actionCollection()->addAction( "previous", prevAction );
  connect( prevAction, SIGNAL(triggered()),
           qxmms, SLOT(prev()));

  KAction *shuffleAction = new KAction( this );
  shuffleAction->setText( i18n("Shuffle") );
  shuffleAction->setIcon( KIcon("media-playlist-shuffle") );
  shuffleAction->setShortcut( Qt::Key_S );
  actionCollection()->addAction( "shuffle", shuffleAction );
  connect( shuffleAction, SIGNAL(triggered()),
           playlist, SLOT(shuffle()));


  KStandardAction::quit(kapp, SLOT(quit()),
                        actionCollection());

  setupGUI();
}
