#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>

#include <lastfm/Track.h>

class Spotify : public QObject
{
    Q_OBJECT
public:
    enum SpotifyError
    {
        TrackNotFound,
        PlaybackError
    };

    explicit Spotify( const QString& username, const QString& password, QObject *parent = 0);

    void login( const QString& username, const QString& password );

    bool loggedIn() const;

    void play( const Track& track );
    void pause();
    void resume();
    void stop();

public:
    void _loginFinished( bool successful );
    void _trackPositionChanged( double seconds );
    void _trackStopped();

signals:
    void loginFinished( bool successful );
    void started( const Track& track );
    void stopped();
    void error( Spotify::SpotifyError error );
    void tick( qint64 );

private slots:
    void onPlaylinks();
private:
    lastfm::Track m_track;
    bool m_loggedIn;
};

#endif // SPOTIFY_H
