#include <boost/variant.hpp>
#include <boost/bind.hpp>
#include "qxmms2.h"
#include "xmmsqt4.h"


QXmms2::QXmms2(QObject *parent, Xmms::Client *xmms )
    : QObject(parent)
{
    //currentId = -1;
    this->xmms = xmms;

    if ( xmms )
        gotConnection( xmms->isConnected() );

}

QXmms2::~QXmms2()
{

}

void
QXmms2::setClient( const Xmms::Client *xmms )
{
    this->xmms = xmms;
    gotConnection ( xmms->isConnected() );
}

void
QXmms2::gotConnection ( bool ok )
{
    if ( ! ok )
        return;

    xmms->playback.getStatus()(
            Xmms::bind( &QXmms2::status_cb, this ));

    xmms->playback.currentID()(
            Xmms::bind( &QXmms2::current_id_cb, this ));

    xmms->playback.broadcastStatus() (
            Xmms::bind( &QXmms2::status_cb, this ));

    xmms->playback.broadcastCurrentID()(
            Xmms::bind( &QXmms2::current_id_cb, this ));

    xmms->playback.signalPlaytime()(
            Xmms::bind(&QXmms2::playtime_cb, this));
}

infoHash
QXmms2::currentInfo()
{
    return _currentInfo;
}


/**
  Slots
 */
void
QXmms2::seek(int ms)
{
    xmms->playback.seekMs(ms);
}

void
QXmms2::playpause()
{
    if ( status == Xmms::Playback::PLAYING )
        xmms->playback.pause();
    else
        xmms->playback.start();
}

void
QXmms2::playpause( bool ok )
{
    if ( ok )
        xmms->playback.start();
    else
        xmms->playback.pause();
}

void
QXmms2::play()
{
    if ( status == Xmms::Playback::PLAYING )
        return;

    xmms->playback.start();
}

void
QXmms2::pause()
{
    xmms->playback.pause();
}

void
QXmms2::stop()
{
    xmms->playback.stop();
}

void
QXmms2::next()
{
    xmms->playlist.setNextRel(1);
    xmms->playback.tickle();
}

void
QXmms2::prev()
{
    xmms->playlist.setNextRel(-1);
    xmms->playback.tickle();
}


/**
  *xmms callbacks
  */
bool
QXmms2::current_id_cb(unsigned int id)
{
    //currentId = id;
    xmms->medialib.getInfo( id )( Xmms::bind( &QXmms2::get_info_cb, this ));
    emit currentIdChanged( id );
    return true;
}

bool
QXmms2::get_info_cb( const Xmms::PropDict &info )
{
    _currentInfo.clear();
    info.each(Xmms::bind( &QXmms2::update_hash, this));
    if ( info.contains("picture_front")){
        std::string hash = info.get< std::string > ( "picture_front" );
        xmms->bindata.retrieve( hash )(
                Xmms::bind(&QXmms2::bindata_retrieve_cb, this));
    }
    if ( info.contains("duration") ){
        int duration = info.get< int > ("duration");
        emit trackDurationChanged( duration );
    }
    emit currentInfoChanged( _currentInfo );
    return true;
}

bool
QXmms2::bindata_retrieve_cb(const Xmms::bin &data)
{
    QPixmap albumArt;
    albumArt.loadFromData( data.c_str(), data.size() );
    emit currentArtChanged(albumArt);
    return true;
}

void
QXmms2::update_hash(const std::string &key,
                        const Xmms::Dict::Variant &value,
                        const std::string &source)
{
    QString qKey = QString::fromStdString(key);
    QVariant qValue;

    if (value.type() == typeid( std::string ))
        qValue = QVariant( QString::fromLocal8Bit(
                boost::get< std::string >( value ).c_str() ));

    else if ( value.type() == typeid( int )){
        qValue = QVariant( boost::get< int >( value ) );
    }
    _currentInfo.insert( qKey, qValue );
}

bool
QXmms2::status_cb( const Xmms::Playback::Status status )
{
    this->status = status;

    if ( status == Xmms::Playback::PLAYING )
        emit serverIsPlaying( true );
    else
        emit serverIsPlaying( false );

    return true;
}

bool
QXmms2::playtime_cb( unsigned int ms )
{
    int position = static_cast< int > ( ms );
    emit trackPositionChanged( position );
    return true;
}
