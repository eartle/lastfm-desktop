
#include <QAction>
#include <QBuffer>

#include <lastfm/Track.h>

#include "../Application.h"
#include "../Services/RadioService/RadioService.h"

#include "CommandReciever.h"

#import <Cocoa/Cocoa.h>

@interface LastfmPlayPauseCommand: NSScriptCommand { } @end
@interface LastfmNextCommand: NSScriptCommand { } @end
@interface LastfmPrevCommand: NSScriptCommand { } @end
@interface LastfmLoveCommand: NSScriptCommand { } @end
@interface LastfmBanCommand: NSScriptCommand { } @end

@implementation LastfmPlayPauseCommand

- (id)performDefaultImplementation
{
    aApp->playAction()->trigger();
    return nil;
}

@end

@implementation LastfmNextCommand

- (id)performDefaultImplementation
{
    aApp->skipAction()->trigger();
    return nil;
}

@end

@implementation LastfmPrevCommand

- (id)performDefaultImplementation
{
    // do nothing for the back button
    return nil;
}

@end

@implementation LastfmLoveCommand

- (id)performDefaultImplementation
{
    aApp->loveAction()->trigger();
    return nil;
}

@end

@implementation LastfmBanCommand

- (id)performDefaultImplementation
{
    aApp->banAction()->trigger();
    return nil;
}

@end

@implementation NSData (LastfmAdditions)

+ (id)scriptingLastfmImageWithDescriptor:(NSAppleEventDescriptor *)descriptor
{
    if ( [descriptor descriptorType] == typeType && [descriptor typeCodeValue] == 'msng' )
    {
        return nil;
    }

    if ( [descriptor descriptorType] != typeTIFF )
    {
        descriptor = [descriptor coerceToDescriptorType: typeTIFF];
        if (descriptor == nil)
        {
            return nil;
        }
    }

    return [descriptor data];
}

- (id)scriptingLastfmImageDescriptor
{
    return [NSAppleEventDescriptor descriptorWithDescriptorType: typeTIFF data: self];
}

@end

CommandReciever::CommandReciever( QObject *parent )
    :QObject( parent ), m_artworkDownloaded( false )
{
    bool success = QDir::home().mkdir( "Library/Application Support/Airfoil/" );
    success = QDir::home().mkdir( "Library/Application Support/Airfoil/RemoteControl/" );
    success = QDir::home().mkdir( "Library/Application Support/Airfoil/TrackTitles/" );

    // make sure the scripts are copied
    success = QFile::copy( QApplication::applicationDirPath() + "/../Resources/dacp.fm.last.Scrobbler.scpt",
                                    QDir::home().filePath( "Library/Application Support/Airfoil/RemoteControl/dacp.fm.last.Scrobbler.scpt" ) );

    success = QFile::copy( QApplication::applicationDirPath() + "/../Resources/fm.last.Scrobbler.scpt",
                                    QDir::home().filePath( "Library/Application Support/Airfoil/TrackTitles/fm.last.Scrobbler.scpt" ) );

    aApp->delegate()->setCommandObserver( this );

    connect( &RadioService::instance(), SIGNAL(trackSpooled(Track)), SLOT(onTrackSpooled(Track)) );
    connect( &RadioService::instance(), SIGNAL(stopped()), SLOT(onStopped()));
}

bool
CommandReciever::artworkDownloaded() const
{
    return m_artworkDownloaded;
}

Track
CommandReciever::track() const
{
    if ( m_trackImageFetcher )
        return m_trackImageFetcher->track();

    return Track();
}

void
CommandReciever::onTrackSpooled( const Track& track )
{
    // if the track has changed or stopped playing get rid of the track fetcher
    if ( m_trackImageFetcher )
    {
        if ( m_trackImageFetcher->track() != track || track.isNull() )
        {
            delete m_trackImageFetcher;
            m_pixmap = QPixmap();
        }
    }

    // if we haven't fetched the new track image yet then do it now
    if ( !m_trackImageFetcher && !track.isNull() )
    {
        m_artworkDownloaded = false;
        m_trackImageFetcher = new TrackImageFetcher( track, Track::MegaImage );
        connect( m_trackImageFetcher, SIGNAL(finished(QPixmap)), SLOT(onFinished(QPixmap)));
        m_trackImageFetcher->startAlbum();
    }
}

void
CommandReciever::onStopped()
{
    delete m_trackImageFetcher;
}

void
CommandReciever::onFinished( const QPixmap& pixmap )
{  
    m_artworkDownloaded = true;
    m_pixmap = pixmap;
}

QPixmap
CommandReciever::getArtwork() const
{
    return m_pixmap;
}

QString
CommandReciever::trackTitle() const
{
    Track t = track();
    QString string;

    if ( !t.isNull() && artworkDownloaded() )
        string = t.title( Track::Corrected );

    return string;
}

QString
CommandReciever::artist() const
{
    Track t = track();
    QString string;

    if ( !t.isNull() && artworkDownloaded() )
        string = t.artist( Track::Corrected );

    return string;
}

QString
CommandReciever::album() const
{
    Track t = track();
    QString string;

    if ( !t.isNull() && artworkDownloaded() )
        string = t.album( Track::Corrected );

    return string;
}

int
CommandReciever::duration()
{
    Track t = track();

    if ( !t.isNull() && artworkDownloaded() )
        return t.duration();

    return 0;
}

QPixmap
CommandReciever::artwork()
{
    Track t = track();

    if ( !t.isNull() )
    {
        QPixmap pixmap = getArtwork();

        if ( !pixmap.isNull() && artworkDownloaded() )
            return pixmap;
    }

    return QPixmap();
}

bool
CommandReciever::loved()
{
    Track t = track();

    if ( !t.isNull() && artworkDownloaded() )
        return t.isLoved();

    return false;
}

CommandReciever::~CommandReciever()
{
}
