#include "playlistmanagerwidget.h"
#include <KIcon>
#include <QModelIndex>

PlaylistManagerWidget::PlaylistManagerWidget( QWidget *parent, Xmms::Client *xmms )
        : KListWidget( parent )
{
    selectedPlaylist = 0;
    this->xmms = xmms;
    activePlaylist = "";
    this->setAlternatingRowColors( true );
    this->setSortingEnabled( true );

    if ( xmms )
        gotConnection( xmms->isConnected() );

    connect( this, SIGNAL(itemActivated(QListWidgetItem*)),
             this, SLOT(entryExecuted(QListWidgetItem*)));
}

void
PlaylistManagerWidget::gotConnection( bool ok )
{
    if ( !ok ){
        setEnabled( false );
        return;
    }

    setEnabled( true );
    xmms->playlist.list()( Xmms::bind( &PlaylistManagerWidget::playlists_list_cb ,this));
    xmms->playlist.currentActive()( Xmms::bind( &PlaylistManagerWidget::current_active_cb, this ));
}

//Public slots
void
PlaylistManagerWidget::newPlaylist()
{

}

void
PlaylistManagerWidget::remove( const QString &name )
{

}

void
PlaylistManagerWidget::entryExecuted( QListWidgetItem *item )
{
    QFont font = selectedPlaylist->font();
    font.setBold( false );
    selectedPlaylist->setFont( font );

    QString playlistName = item->text();
    emit  playlistSelected( playlistName );

    selectedPlaylist = item;

    font = item->font();
    font.setBold( true );
    item->setFont( font );

}

/**
  *xmms callbacks
  */
bool
PlaylistManagerWidget::playlists_list_cb( const Xmms::List< std::string > &list )
{
    QString name;
    for ( Xmms::List< std::string >::const_iterator i(list.begin()),
          i_end(list.end()); i != i_end; ++i ){
        name = QString::fromLocal8Bit( ( *i ).c_str() );
        if ( name.startsWith('_') )
            continue;
        new QListWidgetItem( name, this );
    }
    return true;
}

bool
PlaylistManagerWidget::current_active_cb( const std::string &activePlaylist )
{
    QListWidgetItem *_item;

    this->activePlaylist = activePlaylist;
    for ( int pos = 0; pos < count (); pos++ ){
        _item = item( pos );
        if ( _item->text().toStdString() == activePlaylist ){
            KIcon icon( "media-playback-start" );
            _item->setIcon( icon );
            if ( !selectedPlaylist ){
                selectedPlaylist = _item;
                QFont font = _item->font();
                font.setBold( true );
                _item->setFont( font );
            }
            return true;
        }
    }
    return true;
}
