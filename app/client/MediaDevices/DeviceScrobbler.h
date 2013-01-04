#ifndef DEVICE_SCROBBLER_H_
#define DEVICE_SCROBBLER_H_

#include <QDialogButtonBox>
#include <QProcess>

#include "lib/unicorn/UnicornSession.h"
#include "lib/unicorn/UnicornSettings.h"
#include <lastfm/User.h>
#include "IpodDevice.h"

#ifdef Q_WS_X11
#include <QPointer>
#include "IpodDevice_linux.h"
#endif

using unicorn::Session;

class ScrobbleConfirmationDialog;

class DeviceScrobbler : public QObject
{
    Q_OBJECT
public:
    explicit DeviceScrobbler( QObject* parent = 0 );
    ~DeviceScrobbler();

signals:
    void foundScrobbles( const QList<lastfm::Track>& tracks );

public slots:
#ifdef Q_WS_X11
    void onScrobbleIpodTriggered();
#endif

private slots:
#ifdef Q_WS_X11
    void onCalculatingScrobbles( int trackCount );
    void scrobbleIpodTracks( int trackCount );
    void onIpodScrobblingError();
#endif

    void twiddle();
    void onTwiddlyFinished( int, QProcess::ExitStatus );
    void onTwiddlyError( QProcess::ProcessError );

    void onScrobblesConfirmationFinished( int result );
    void checkCachedIPodScrobbles();

public:
    void handleMessage( const QStringList& );
    void iPodDetected( const QStringList& arguments );

private:
#ifdef Q_WS_X11
    QPointer<IpodDeviceLinux> iPod;
#endif
    bool isITunesPluginInstalled();
    void twiddled( const QStringList& arguments );
    void scrobbleIpodFiles( const QStringList& files );
    QList<lastfm::Track> scrobblesFromFiles( const QStringList& files );

    lastfm::User associatedUser( QString deviceId );

private:
    QPointer<QProcess> m_twiddly;
    QTimer* m_twiddlyTimer;
    QPointer<ScrobbleConfirmationDialog> m_confirmDialog;
};

#endif //DEVICE_SCROBBLER_H_
