#include "dplaylistmanagerwidget.h"

#include "playlistmanagerwidget.h"
#include <KIcon>
#include <QModelIndex>

DPlaylistManagerWidget::DPlaylistManagerWidget( QWidget *parent, Xmms::Client *xmms )
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
DPlaylistManagerWidget::gotConnection( bool ok )
{
    if ( !ok ){
        setEnabled( false );
        return;
    }

    setEnabled( true );
    xmms->collection.list( Xmms::Collection::COLLECTIONS ) (
            Xmms::bind( &DPlaylistManagerWidget::collections_list_cb ,this) );
    //xmms->playlist.currentActive()( Xmms::bind( &DPlaylistManagerWidget::current_active_cb, this ));
}

//Public slots
/*void
DPlaylistManagerWidget::newPlaylist()
{

}

void
DPlaylistManagerWidget::remove( const QString &name )
{

}
*/
void
DPlaylistManagerWidget::entryExecuted( QListWidgetItem *item )
{
    QString playlistName = item->text();

    xmms->playlist.clear( "_musik" );
    xmms->collection.get( playlistName.toStdString(), Xmms::Collection::COLLECTIONS )(
            Xmms::bind( &DPlaylistManagerWidget::collection_get_cb, this ));

    QFont font;

    if ( selectedPlaylist ){
        font = selectedPlaylist->font();
        font.setBold( false );
        selectedPlaylist->setFont( font );
    }

    emit  dPlaylistSelected( playlistName );

    selectedPlaylist = item;

    font = item->font();
    font.setBold( true );
    item->setFont( font );

}

/**
  *xmms callbacks
  */
bool
DPlaylistManagerWidget::collections_list_cb( const Xmms::List< std::string > &list )
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
DPlaylistManagerWidget::collection_get_cb( const Xmms::Coll::Coll &collection )
{
    xmms->playlist.addCollection( collection,
                                  std::list< std::string > (),
                                  "_musik" );
    return true;
}

/*
bool
DPlaylistManagerWidget::current_active_cb( const std::string &activePlaylist )
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
*/
