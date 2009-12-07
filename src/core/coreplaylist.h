#ifndef COREPLAYLIST_H
#define COREPLAYLIST_H
#include <xmmsclient/xmmsclient++.h>
#include <QObject>
#include <QHash>
#include <QList>
#include <QVariant>
#include <QPixmap>

typedef QHash < QString, QVariant > entry_t;

/**
 *CorePlaylist is intended as a cache, storing relevant metadata for any model of the playlist
 */
class CorePlaylist : public QObject
{
    Q_OBJECT

public:
    /**
      * if playlistName is omited, CorePlaylist will represent the current playlist in the server
      */
    CorePlaylist(QObject *parent = 0, Xmms::Client *xmms = 0, QString playlistName = "");
    void setPlaylist( const QString &playlistName );
    void setClient( Xmms::Client *xmms );

    QString playlistName();
    bool active();
    void activate( int pos = -1 );

    //void insert( QString url );
    void add( QString url );
    void remove( int pos );
    void move ( unsigned int oldPos, unsigned int newPos );

    void sort( QString property );
    void shuffle();

    /**
      * set keys whose values will be stored in the cache.
      *
      * Note that if no keys are set (default) CorePlaylist will store the whole metainfo returned
      * by the server. Also, storing "picture_front" implies that CorePlaylist will cache album art.
      */
    void setKeys( QList< QString > keys );

    entry_t entry( int position ) const;
    QVariant value ( int position, QString key ) const;
    QList < entry_t > entries() const;
    QPixmap art( QString hash );
    unsigned int currentPosition ();
    int rowCount();

signals:
    void rowsAboutToBeInserted( int, int );
    void rowsInserted ();
    void rowsAboutToBeRemoved( int, int );
    void rowsRemoved ();
    void dataChanged ( int, QString );
    void positionChanged ( int );

private:
    Xmms::Client *xmms;
    std::string _playlistName;
    bool isActive;
    unsigned int currentPos;
    QList< entry_t  > cache;
    QHash< QString, QPixmap > artCache;
    QList< QString > keys;

    void gotConnection( bool ok );

    bool list_entries_cb( const Xmms::List< int > &list );
    bool get_info_cb( const Xmms::PropDict &info, int playlistPos = -1 );
    bool update_info_cb( const Xmms::PropDict &info );
    bool for_each_fcn (const std::string &key, const Xmms::Dict::Variant &value,
                       const std::string &origin, int position );
    bool bindata_retrieve_cb( const std::string &hash, const Xmms::bin &data );
    bool current_pos_cb( const Xmms::Dict &info );
    bool playlist_changed_cb( const Xmms::Dict &info );
    bool current_active_cb ( const std::string &activeName );
    bool entry_changed_cb ( const unsigned int id );
};

#endif // COREPLAYLIST_H
