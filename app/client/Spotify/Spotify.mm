
#include <QDebug>
#include <QNetworkReply>
#include <QTimer>

#include <CocoaLibSpotify/CocoaLibSpotify.h>

#include <lastfm/Track.h>
#include <lastfm/XmlQuery.h>

#include "Spotify.h"

#import <Cocoa/Cocoa.h>
#import <CocoaLibSpotify/CocoaLibSpotify.h>

#include <stdint.h>
#include <stdlib.h>
const uint8_t g_appkey[] = {
    0x01, 0xEB, 0x81, 0x30, 0xE1, 0x7C, 0x08, 0x83, 0x67, 0x1B, 0x7A, 0x84, 0xD9, 0x52, 0xDC, 0x76,
    0x3E, 0xC0, 0x9F, 0xB3, 0xBC, 0x64, 0x1F, 0x11, 0x85, 0xA9, 0xB3, 0x43, 0xFF, 0xA3, 0xD4, 0xF9,
    0x54, 0x96, 0x25, 0x77, 0xC3, 0x3D, 0x77, 0x0B, 0x18, 0x98, 0x62, 0x04, 0xB1, 0x75, 0xFB, 0x4B,
    0x7A, 0xAA, 0x59, 0xD5, 0xFD, 0x16, 0xBF, 0xA2, 0x78, 0xB6, 0xF6, 0xDF, 0x06, 0x94, 0xE6, 0x9F,
    0xD0, 0x9A, 0xE4, 0x50, 0x2E, 0xEF, 0x70, 0x0B, 0x37, 0xC7, 0x23, 0xA2, 0xF9, 0xBC, 0x5E, 0x73,
    0x76, 0x8F, 0xF6, 0xF2, 0xB0, 0xAA, 0xF6, 0x48, 0x13, 0x5B, 0x79, 0xE9, 0xB1, 0x47, 0x16, 0xD5,
    0x3D, 0xED, 0x8A, 0x1F, 0x13, 0x84, 0x2B, 0x84, 0x9F, 0xB4, 0x2C, 0x10, 0x34, 0x01, 0x17, 0x63,
    0xE7, 0xA0, 0xE7, 0x15, 0xA9, 0x9C, 0x64, 0xFF, 0x45, 0x24, 0x90, 0x15, 0x5D, 0xDB, 0x14, 0x3F,
    0x29, 0x17, 0x87, 0xEC, 0xF4, 0x03, 0x6F, 0x7E, 0x45, 0x87, 0x8B, 0x06, 0xBD, 0xC0, 0x1B, 0xCB,
    0x54, 0xF2, 0xD1, 0xFE, 0x4C, 0x7A, 0x90, 0x29, 0x58, 0x1D, 0x1F, 0xE4, 0x68, 0xAE, 0x6E, 0x6C,
    0x15, 0x6D, 0x46, 0x11, 0xA0, 0xBE, 0x2E, 0xC1, 0x31, 0x86, 0x02, 0x56, 0x13, 0xE0, 0x29, 0xD2,
    0xAF, 0x94, 0x6E, 0xBE, 0x04, 0x27, 0x9F, 0x37, 0xF3, 0x92, 0x4E, 0x17, 0xF6, 0x27, 0x73, 0x24,
    0x7A, 0xA7, 0x7B, 0x07, 0x4B, 0x2F, 0x81, 0xC7, 0x7E, 0x30, 0x5D, 0x43, 0xBD, 0x45, 0x7E, 0xB5,
    0x43, 0x86, 0x03, 0x53, 0x76, 0x53, 0x09, 0x54, 0x02, 0x4A, 0x1B, 0xD0, 0xE1, 0xBA, 0x0C, 0x00,
    0xFF, 0xDB, 0xA1, 0xAE, 0xBB, 0x52, 0xA4, 0x0C, 0xEF, 0x26, 0xEC, 0x76, 0x33, 0x70, 0xE9, 0x9C,
    0xBC, 0xAA, 0x21, 0x1D, 0x98, 0x14, 0xF0, 0xA1, 0xBF, 0x44, 0x65, 0xEF, 0x90, 0xAB, 0x4D, 0x10,
    0x54, 0x57, 0xBF, 0x45, 0xC7, 0x33, 0x79, 0xCC, 0x02, 0x68, 0x85, 0x1C, 0x10, 0xAC, 0x9C, 0x7B,
    0x69, 0x6A, 0x87, 0x7E, 0x52, 0x66, 0x79, 0x2D, 0x66, 0xCA, 0xFC, 0x96, 0x05, 0xBC, 0x1E, 0xC8,
    0xEC, 0xA2, 0x08, 0xAC, 0x1A, 0xB2, 0xDA, 0x48, 0xDD, 0x09, 0x2C, 0xA2, 0xCA, 0xAB, 0xC1, 0x62,
    0xFC, 0x85, 0xA1, 0x7C, 0x3A, 0x43, 0x5B, 0x11, 0x20, 0xF6, 0xFE, 0x9B, 0xFE, 0x33, 0xAA, 0xCC,
    0x5D,
};
const size_t g_appkey_size = sizeof(g_appkey);

@interface SimplePlayerDelegate : NSObject  <SPSessionDelegate, SPSessionPlaybackDelegate> {
@private
    SPPlaybackManager* m_playbackManager;
    Spotify* m_observer;
}

- (SimplePlayerDelegate*) initialise:(Spotify*)observer;

@property (nonatomic, readwrite, strong) SPPlaybackManager* m_playbackManager;

@end

@implementation SimplePlayerDelegate

@synthesize m_playbackManager;

- (SimplePlayerDelegate*) initialise:(Spotify*)observer
{
    if ( (self = [super init]) )
    {
        self->m_observer = observer;
    }

    [self addObserver:self
           forKeyPath:@"m_playbackManager.trackPosition"
              options:0
              context:nil];

    return self;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    // Invoked when the current playback position changed (see below). This is a bit of a workaround
    // to make sure we don't update the position slider while the user is dragging it around. If the position
    // slider was read-only, we could just bind its value to playbackManager.trackPosition.

    if ([keyPath isEqualToString:@"m_playbackManager.trackPosition"])
    {
        m_observer->_trackPositionChanged( self.m_playbackManager.trackPosition );
    }
}

-(void)sessionDidLoginSuccessfully:(SPSession *)aSession
{
    m_observer->_loginFinished( true );
    qDebug() << "Loggin sucess! :)";
}

-(void)session:(SPSession *)aSession didFailToLoginWithError:(NSError *)error
{
    qDebug() << "Logged failed! :(";
    m_observer->_loginFinished( false );
}

-(void)sessionDidLogOut:(SPSession *)aSession
{
}

-(void)session:(SPSession *)aSession didEncounterNetworkError:(NSError *)error
{
}

-(void)session:(SPSession *)aSession didLogMessage:(NSString *)aMessage
{
}

-(void)sessionDidChangeMetadata:(SPSession *)aSession
{
}

-(void)session:(SPSession *)aSession recievedMessageForUser:(NSString *)aMessage
{
    [[NSAlert alertWithMessageText:aMessage
                     defaultButton:@"OK"
                   alternateButton:@""
                       otherButton:@""
         informativeTextWithFormat:@"This message was sent to you from the Spotify service."] runModal];
}

-(void)sessionDidEndPlayback:(id<SPSessionPlaybackProvider>)aSession
{
    m_observer->_trackStopped();
}

@end

SimplePlayerDelegate* g_delegate;

Spotify::Spotify( const QString& username, const QString& password, QObject *parent) :
    QObject(parent), m_loggedIn( false )
{
    [SPSession initializeSharedSessionWithApplicationKey:[NSData dataWithBytes:&g_appkey length:g_appkey_size]
                                               userAgent:@"com.spotify.SimplePlayer"
                                           loadingPolicy:SPAsyncLoadingManual
                                                   error:nil];

    g_delegate = [[SimplePlayerDelegate alloc] initialise:this];

    g_delegate.m_playbackManager = [[SPPlaybackManager alloc] initWithPlaybackSession:[SPSession sharedSession]];

    [[SPSession sharedSession] setDelegate:g_delegate];
    [[SPSession sharedSession] setPlaybackDelegate:g_delegate];

    [g_delegate addObserver:g_delegate
           forKeyPath:@"m_playbackManager.trackPosition"
              options:0
              context:nil];

    login( username, password );
}

NSString*
nsString( const QString& string )
{
    return [NSString stringWithCharacters:(const unichar *)string.unicode() length:(NSUInteger)string.length() ];
}


void
Spotify::login( const QString& username, const QString& password )
{
    m_loggedIn = false;
    [[SPSession sharedSession] attemptLoginWithUserName:nsString( username ) password: nsString( password ) ];
}

void
Spotify::_loginFinished( bool successful )
{
    m_loggedIn = successful;
    emit loginFinished( successful );
}

bool
Spotify::loggedIn() const
{
    return m_loggedIn;
}

QString
qt_mac_NSStringToQString( const NSString* nsstr )
{
    NSRange range;
    range.location = 0;
    range.length = [nsstr length];

    unichar *chars = new unichar[range.length];
    [nsstr getCharacters:chars range:range];
    QString result = QString::fromUtf16(chars, range.length);
    delete[] chars;
    return result;
}

void
Spotify::onGotPlaylinks()
{
    lastfm::XmlQuery lfm;

    // get the spotify ids from the xml
    if ( lfm.parse( static_cast<QNetworkReply*>( sender() ) ) )
    {
        QString spotifyId = lfm["spotify"]["track"]["externalids"]["spotify"].text();

        if ( !spotifyId.isEmpty() )
        {
            MutableTrack( m_track ).setExtra( "spotifyId", spotifyId );
            doPlay();
        }
        else
            emit error( PlaybackError, 0, "" );
    }
}

void
Spotify::play( Track& track )
{
    pause();
    m_track = track;

    if ( m_track.extra( "spotifyId" ) == "unknown" )
    {
        // fetch the playlink
        QList<Track> tracks;
        tracks << m_track;
        connect( Track::playlinks( tracks ), SIGNAL(finished()), SLOT(onGotPlaylinks()));
    }
    else
        doPlay();
}

void
Spotify::doPlay()
{
    NSString* id = nsString( m_track.extra( "spotifyId" ) );

    NSURL *trackURL = [NSURL URLWithString:id];

    [[SPSession sharedSession] trackForURL:trackURL callback:^(SPTrack* spTrack)
    {
        if (spTrack != nil)
        {
            [SPAsyncLoading waitUntilLoaded:spTrack timeout:kSPAsyncLoadingDefaultTimeout then:^(NSArray *tracks, NSArray *notLoadedTracks)
            {
                NSTimeInterval duration = [spTrack duration];
                MutableTrack mt( m_track );
                mt.setTitle( qt_mac_NSStringToQString( [spTrack name] ) );
                mt.setAlbum( qt_mac_NSStringToQString( [[spTrack album] name] ) );
                mt.setArtist( qt_mac_NSStringToQString( [[[spTrack artists] objectAtIndex:0] name] ) );
                mt.setDuration( duration );

                [g_delegate.m_playbackManager playTrack:spTrack callback:^(NSError* nsError)
                {
                    if ( nsError != nil )
                        emit error( PlaybackError, [nsError code], qt_mac_NSStringToQString( [nsError localizedDescription] ) );
                    else
                    {
                        MutableTrack( m_track ).setExtra( "streamSource", "Spotify" );
                        emit started( m_track );
                    }
                }];
            }];
        }
        else
            emit error( TrackNotFound, 0, "" );
    }];
}

void
Spotify::_trackPositionChanged( double seconds )
{
    emit tick( seconds * 1000 );
}

void
Spotify::_trackStopped()
{
    emit stopped();
}

void
Spotify::pause()
{
    [g_delegate.m_playbackManager setIsPlaying:false];
}

void
Spotify::resume()
{
    [g_delegate.m_playbackManager setIsPlaying:true];
}

void
Spotify::stop()
{

}
