
#ifndef MEDIA_KEY_H
#define MEDIA_KEY_H

#include <QApplication>
#include <QObject>

#include <lastfm/Track.h>

namespace unicorn { class Session; }

class MediaKey : public QObject
{
    Q_OBJECT
public:
    explicit MediaKey( QObject* parent );

    void setEnabled( bool enabled );

    bool macEventFilter( EventHandlerCallRef, EventRef event );

private slots:
    void initialize();
    void applicationDidFinishLaunching( void* aNotification );
    void onSessionChanged( const unicorn::Session& session );
    void onTrackStarted( const Track& newTrack, const Track& oldTrack );

private:
    bool m_lastTrackRadio;
};

#endif // MEDIA_KEY_H
