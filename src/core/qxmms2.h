#ifndef QXMMS2_H
#define QXMMS2_H
#include <xmmsclient/xmmsclient++.h>
#include <QObject>
#include <QHash>
#include <QPixmap>
#include "types.h"

/**
 *QXmms2 is an abstraction of the properties of a media player, that is to say, basic
 *playback control and current media information. Please note that playlist management
 *is going to be implemented in a separate class for clarity.
 */
class QXmms2 : public QObject
{
    Q_OBJECT

public:
    QXmms2(QObject *parent = 0, Xmms::Client *xmms = 0 );
    ~QXmms2();

    void setClient( const Xmms::Client *xmms );
    infoHash currentInfo();

public slots:
    void seek( int ms ); //seek is not implemented yet!
    void playpause();
    void play();
    void pause();
    void next();
    void prev();
    void stop();

signals:
    void currentIdChanged( unsigned int id );
    void currentInfoChanged( infoHash );
    void currentArtChanged( QPixmap );

private:
    const Xmms::Client *xmms;
    Xmms::Playback::Status status;
    //unsigned int currentId;
    infoHash _currentInfo;
    void gotConnection ( bool ok );

    bool current_id_cb( unsigned int id );
    bool get_info_cb( const Xmms::PropDict &info );
    bool bindata_retrieve_cb( const Xmms::bin &data );
    bool status_cb( const Xmms::Playback::Status status );
    void update_hash( const std::string &key,
                     const Xmms::PropDict::Variant &value,
                     const std::string &source);
};

#endif // QXMMS2_H
