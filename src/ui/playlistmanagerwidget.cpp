#include "playlistmanagerwidget.h"
#include <KIcon>

PlaylistManagerWidget::PlaylistManagerWidget( QWidget *parent, Xmms::Client *xmms )
        : KListWidget( parent )
{
    this->xmms = xmms;
    activePlaylist = "";
    this->setAlternatingRowColors( true );
    this->setSortingEnabled( true );

    if ( xmms )
        gotConnection( xmms->isConnected() );
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

bool
PlaylistManagerWidget::playlists_list_cb( const Xmms::List< std::string > &list )
{
    QString name;
    for ( list.first(); list.isValid(); ++list ){
        name = QString::fromLocal8Bit( ( *list ).c_str() );
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

    for ( int pos = 0; pos < count () ; pos++ ) {
        _item = item( pos );
        if ( _item->text().toStdString() == activePlaylist ){
            QFont font = _item->font();
            font.setBold( false );
            _item->setFont( font );
            _item->setIcon( QIcon() );
            break;
        }
    }

    this->activePlaylist = activePlaylist;
    for ( int pos = 0; pos < count (); pos++ ){
        _item = item( pos );
        if ( _item->text().toStdString() == activePlaylist ){
            QFont font = _item->font();
            font.setBold( true );
            _item->setFont( font );
            KIcon icon( "media-playback-start" );
            _item->setIcon( icon );
            return true;
        }
    }
    return true;
}
