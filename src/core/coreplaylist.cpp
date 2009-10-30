#include <boost/bind.hpp>
#include "coreplaylist.h"

/**
 *Contructor: After you create this object
 * you must invoke setKeys sometime to load the playlist
 */
CorePlaylist::CorePlaylist( QObject *parent, Xmms::Client *xmms, QString playlistName)
        : QObject( parent )
{
    this->xmms = xmms;

    if ( playlistName.isEmpty() )
        isActive = true;
    else {
        isActive = false;
        this->_playlistName = playlistName.toStdString();
    }
}

QString
CorePlaylist::playlistName()
{
    return QString::fromStdString( _playlistName );
}

void
CorePlaylist::setPlaylist( const QString &playlistName )
{
    if ( playlistName.toStdString() == xmms->playlist.DEFAULT_PLAYLIST ){
        isActive = true;
        xmms->playlist.currentActive() (
                Xmms::bind (&CorePlaylist::current_active_cb, this ));
    }
    else{
        isActive = false;
        _playlistName = playlistName.toStdString();
    }
    emit rowsAboutToBeRemoved( 0, cache.size() - 1 );
    cache.clear();
    emit rowsRemoved ();
    xmms->playlist.listEntries( _playlistName )(
                Xmms::bind ( &CorePlaylist::list_entries_cb, this ));
}

bool
CorePlaylist::active()
{
    return isActive;
}

void
CorePlaylist::activate( int pos )
{
    if ( !isActive )
        xmms->playlist.load( _playlistName );

    if ( pos > -1 ){
        xmms->playlist.setNext( pos );
        xmms->playback.tickle();
    }
}

void
CorePlaylist::add( QString url )
{
    xmms->playlist.addUrl( url.toStdString(), _playlistName );
}

void
CorePlaylist::remove( int pos )
{
    xmms->playlist.removeEntry( pos, _playlistName );
}

void
CorePlaylist::move( unsigned int oldPos, unsigned int newPos )
{
    xmms->playlist.moveEntry( oldPos, newPos, _playlistName );
}

void
CorePlaylist::sort( QString property )
{
    xmms->playlist.sort(
            std::list < std::string > ( 1, property.toStdString() ),
            _playlistName );

}

void
CorePlaylist::setKeys( QList< QString > keys )
{
    this->keys = keys;
    cache.clear();
    if ( xmms )
        gotConnection ( xmms->isConnected() );
}

void
CorePlaylist::gotConnection( bool ok )
{
    if ( ok ){
        if ( isActive ){
            _playlistName = xmms->playlist.DEFAULT_PLAYLIST;//ugly hack!
            xmms->playlist.currentActive() (
                Xmms::bind (&CorePlaylist::current_active_cb, this ));
        }
        xmms->playlist.listEntries( _playlistName )(
                Xmms::bind ( &CorePlaylist::list_entries_cb, this ));
        xmms->playlist.currentPos( _playlistName )(
                Xmms::bind (&CorePlaylist::current_pos_cb, this));
        xmms->playlist.broadcastCurrentPos() (
                Xmms::bind (&CorePlaylist::current_pos_cb, this));
        xmms->playlist.broadcastChanged() (
                Xmms::bind (&CorePlaylist::playlist_changed_cb, this));
        xmms->playlist.broadcastLoaded()(
                Xmms::bind (&CorePlaylist::current_active_cb, this ));
        xmms->medialib.broadcastEntryChanged()(
                Xmms::bind( &CorePlaylist::entry_changed_cb, this ));
    }
}

entry_t
CorePlaylist::entry( int position ) const
{
    return cache[position];
}

QVariant
CorePlaylist::value( int position, QString key ) const
{
    if ( (position < 0) || (position > cache.size())
        || !(keys.contains( key )))
        return QVariant();

    entry_t entry = cache [ position ];
    return entry [ key ];
}

QList < entry_t >
CorePlaylist::entries() const{
    return cache;
}

QPixmap
CorePlaylist::art( QString hash ){
    return artCache[ hash ];
}

unsigned int
CorePlaylist::currentPosition()
{
    return currentPos;
}

int
CorePlaylist::rowCount()
{
    return cache.size();
}

bool
CorePlaylist::list_entries_cb( const Xmms::List< unsigned int > &list )
{
    for ( list.first(); list.isValid(); ++list ) {
        xmms->medialib.getInfo( *list )( boost::bind( &CorePlaylist::get_info_cb, this, _1, -1));
    }
    return true;
}

bool
CorePlaylist::get_info_cb( const Xmms::PropDict &info, int playlistPos )
{
    if ( keys.isEmpty() ){
        emit rowsAboutToBeInserted( cache.size(), cache.size() );
        cache.append( entry_t () );
        info.each( boost::bind( &CorePlaylist::for_each_fcn, this, _1, _2, _3, cache.size() - 1 ) );
        emit rowsInserted ();
    }
    else{
        entry_t entry;
        QVariant qvalue;
        Xmms::Dict::Variant xvalue;
        std::string xkey;

        foreach (QString key, keys ){
            xkey = key.toStdString();
            if ( info.contains( xkey ) ){
                xvalue = info[xkey];
                if ( xvalue.type() == typeid( unsigned int ) )
                    qvalue = boost::get< unsigned int >( xvalue );
                else if ( xvalue.type() == typeid( int ) )
                    qvalue = boost::get< int >( xvalue );
                else if ( xvalue.type() == typeid( std::string ) ){
                    std::string svalue = boost::get< std::string >( xvalue );
                    qvalue = QString::fromLocal8Bit( svalue.c_str() );
                }
                else
                    qvalue = QVariant();
                entry.insertMulti( key, qvalue);
            }
        }
        if ( playlistPos == -1 ) {
            emit rowsAboutToBeInserted( cache.size(), cache.size() );
            cache.append( entry );
            emit rowsInserted ();
        }
        else {
            emit rowsAboutToBeInserted( playlistPos, playlistPos );
            cache.insert( playlistPos, entry );
            emit rowsInserted ();
        }
    }

    if ( info.contains( "picture_front" )
         && ( keys.isEmpty() || keys.contains( "picture_front" ) )) {

        std::string hash = info.get< std::string >("picture_front");

        if ( artCache.contains( QString::fromStdString( hash ) ) )
            return true;

        xmms->bindata.retrieve( hash )(boost::bind( &CorePlaylist::bindata_retrieve_cb, this,
                                                    hash, _1 ));
    }

    return true;
}

/**
  *this function is called whenever some entries change in the playlist.
  */
bool
CorePlaylist::update_info_cb( const Xmms::PropDict &info )
{
    unsigned int id = info.get< unsigned int >("id");
    entry_t entry;
    std::string xkey;
    QVariant value;
    Xmms::Dict::Variant xvalue;

    for ( int pos = 0; pos < cache.size(); pos++ ){
        entry = cache.at( pos );
        if ( entry.value( "id" ) == id ){
            foreach ( QString key, entry.keys() ){
                xkey = key.toStdString();
                if ( info.contains( xkey ) ){
                    xvalue = info[ xkey ];
                    if ( xvalue.type() == typeid( std::string ) )
                        value = QString::fromLocal8Bit(
                                boost::get< std::string > ( xvalue ).c_str() );
                    else if ( xvalue.type() == typeid( int ) )
                        value = boost::get< int > ( xvalue );
                    else if ( xvalue.type() == typeid( unsigned int ) )
                        value = boost::get< unsigned int > ( xvalue );
                    entry.insert( key, value );
                    emit dataChanged( pos, key );
                }
            }
        }
    }
    return true;
}

bool
CorePlaylist::for_each_fcn( const std::string &key, const Xmms::Dict::Variant &value,
                            const std::string &origin, int  position )
{       
    QVariant qvalue;

    if ( value.type() == typeid( unsigned int ) )
        qvalue = boost::get< unsigned int >( value );
    else if ( value.type() == typeid( int ) )
        qvalue = boost::get< int >( value );
    else if ( value.type() == typeid( std::string ) )
        qvalue = QString::fromStdString( boost::get< std::string >( value ) );
    else
        return true;

    entry_t entry = cache[position];
    entry.insertMulti( QString::fromStdString( key ), qvalue );

    return true;
}

bool
CorePlaylist::bindata_retrieve_cb( const std::string &hash, const Xmms::bin &data )
{
    QPixmap albumArt;
    albumArt.loadFromData(data.c_str(), data.size());
    artCache[ QString::fromStdString( hash ) ] = albumArt;
    return true;
}


bool
CorePlaylist::current_pos_cb( const Xmms::Dict &info)
{
    std::string plname = info.get<std::string>( "name" );
    if ( _playlistName == info.get<std::string>( "name" )){
        currentPos = info.get< unsigned int >("position");
        emit positionChanged ( currentPos );
    }
    return true;
}

bool
CorePlaylist::current_active_cb( const std::string &activeName )
{
    if ( isActive )
        _playlistName = activeName;
    return true;
}

bool
CorePlaylist::entry_changed_cb( const unsigned int id )
{
    foreach( entry_t entry, cache )
        if ( entry.value("id") == id ){
            xmms->medialib.getInfo( id )( Xmms::bind( &CorePlaylist::update_info_cb, this));
            break;
        }
    return true;
}

bool
CorePlaylist::playlist_changed_cb( const Xmms::Dict &info )
{
    std::string playlist = info.get< std::string >("name");

    if ( _playlistName != playlist )
        return true;

    int changeType = info.get<int>("type"),
        pos, newPos;
    unsigned int id;

    entry_t entry;
    int lastPos;

    if ( info.contains("position") )
        pos = info.get< int >( "position" );
    if ( info.contains( "id" ) )
        id = info.get< unsigned int >( "id" );

    switch (changeType)
    {
        case XMMS_PLAYLIST_CHANGED_ADD:
        xmms->medialib.getInfo( id )( boost::bind( &CorePlaylist::get_info_cb, this, _1, -1 ));
        break;

        case XMMS_PLAYLIST_CHANGED_INSERT:
        xmms->medialib.getInfo( id )( boost::bind(&CorePlaylist::get_info_cb, this, _1, pos ));
        break;

        case XMMS_PLAYLIST_CHANGED_MOVE:
        newPos = info.get< int >( "newposition" );
        cache.move( pos, newPos );
        break;

        case XMMS_PLAYLIST_CHANGED_REMOVE:
        emit rowsAboutToBeRemoved ( pos, pos );
        cache.removeAt(pos);
        emit rowsRemoved ();
        break;

        case XMMS_PLAYLIST_CHANGED_UPDATE:

        break;

        case XMMS_PLAYLIST_CHANGED_SHUFFLE:
        case XMMS_PLAYLIST_CHANGED_SORT:
        case XMMS_PLAYLIST_CHANGED_CLEAR:
        lastPos = cache.size() - 1;
        emit rowsAboutToBeRemoved ( 0,  lastPos );
        cache.clear();
        emit rowsRemoved ();
        xmms->playlist.listEntries( _playlistName )(
                Xmms::bind(&CorePlaylist::list_entries_cb, this));
        // this is puzzling! why the client doesn't grab the signal
        // for itself?
        xmms->playlist.currentPos( _playlistName ) (
                Xmms::bind( &CorePlaylist::current_pos_cb, this ) );
        break;
    }
    return true;
}
