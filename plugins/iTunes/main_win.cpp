/*
   Copyright 2004 Gareth Simpson <iscrobbler@xurble.org>
   Copyright 2004-2009 Last.fm Ltd. 

   This file is part of the Last.fm Desktop Application Suite.

   lastfm-desktop is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   lastfm-desktop is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with lastfm-desktop.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

#include "common/c++/logger.h"
#include "ITunesComThread.h"
#include "Moose.h"
#include "ScrobSubmitter.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cwctype>
#include <cstdlib>


// WTL things
HANDLE gHModule; // WTL handle populated in DLLMain

extern ScrobSubmitter gSubmitter;
extern ITunesComThread* gCom;

// reporting things
std::wstring gStatusStringX; // just some temp debug info (even for the release build) 
std::wstring gErrStringX; // an error

// the current track info
std::wstring gArtist;
std::wstring gAlbumArtist;
std::wstring gTrack;
std::wstring gAlbum;
std::wstring gPath;
std::wstring gKind;
int gLen = 0;
int gStars = 0;
int gBitrate = 0;

DWORD gTimeOfLastStop = 0;


enum EAsState
{
    AS_STOPPED,
    AS_PLAYING,
    AS_PAUSED
};

EAsState gASState = AS_STOPPED;


/******************************************************************************
 * Utility functions                                                          *
 ******************************************************************************/

void
SetError( const std::wstring& err )
{
    LOGWL( 1, err );

    gErrStringX = err;
}


void
SetStatus( const std::wstring& stat )
{
    LOGWL( 3, stat );

    gStatusStringX = stat;
}


void
ClearMemory( LogicalAddress dest, SInt32 length )
{
    register unsigned char *ptr;

    ptr = (unsigned char *) dest;

    if( length > 16 )
    {
        register unsigned long  *longPtr;

        while( ((unsigned long) ptr & 3) != 0 )
        {
            *ptr++ = 0;
            --length;
        }

        longPtr = (unsigned long *) ptr;

        while( length >= 4 )
        {
            *longPtr++  = 0;
            length      -= 4;
        }

        ptr = (unsigned char *) longPtr;
    }

    while( --length >= 0 )
    {
        *ptr++ = 0;
    }
}


static bool
TryMemory( wchar_t* ptr )
{
    wchar_t character;
    __try
    {
        character = *ptr;
    }
    __except( EXCEPTION_EXECUTE_HANDLER )
    {
        return false;
    }
    return true;
}

void
LogTrack()
{
    LOGWL( 3, "Artist: " << gArtist << "\n  " <<
        "Album Artist: " << gAlbumArtist << "\n  " <<
        "Track: " << gTrack << "\n  " <<
        "Album: " << gAlbum << "\n  " <<
        "Length: " << gLen << "\n  " <<
        "Path: " << gPath << "\n  " <<
        "Kind: " << gKind << "\n  " <<
        "Bitrate: " << gBitrate );
}


/*******************************************************************************
 * Scrobbling functions                                                        *
 *******************************************************************************/

/** submits the current song to audioscrobbler */
static void ASStart()
{
    int id = gSubmitter.Start( Moose::wStringToUtf8( gArtist ),
                               Moose::wStringToUtf8( gAlbumArtist ),
                               Moose::wStringToUtf8( gTrack ),
                               Moose::wStringToUtf8( gAlbum ),
                               "",
                               gLen,
                               Moose::wStringToUtf8( gPath ),
                               ScrobSubmitter::UTF_8 );

    std::wostringstream os;
    os << L"ReqID " << id << ": " << L"Sent Start for "
       << gArtist << " - " << gTrack;
    LOGWL( 3, os.str() );

    gASState = AS_PLAYING;
}


static void ASStop()
{
    int id = gSubmitter.Stop();

    std::ostringstream os;
    os << "ReqID " << id << ": " << "Sent Stop";
    LOGL( 3, os.str() );

    gArtist = L"";
	gAlbumArtist = L"";
    gTrack = L"";

    gASState = AS_STOPPED;
}

static void StopCallback()
{
	// This gets called if we recieved a stop message
	// and it's an actaul stop (not a pause)

	// check that we are in the paused state before calling stop
	// there's a small chance if a race condition where we may
	// have resumed in the meantime
	if ( gASState == AS_PAUSED )
		ASStop();
}


static void ASPause()
{
    int id = gSubmitter.Pause();

    std::ostringstream os;
    os << "ReqID " << id << ": " << "Sent Pause";
    LOGL( 3, os.str());

    gASState = AS_PAUSED;
}


static void ASResume()
{
    int id = gSubmitter.Resume();

    LOGL( 3, "ReqID " << id << ": " << "Sent Resume" );

    gASState = AS_PLAYING;
}


void ScrobSubCallback(int reqID, bool error, std::string message, void* userData)
{
#ifdef _DEBUG
    if (error)
    {
        MessageBox( NULL, message.c_str(), NULL, MB_OK );
    }
#endif

    LOGL( 3, "ReqID " << reqID << ": " << message );
}


/*******************************************************************************
 * Our own functions                                                           *
 *******************************************************************************/

static void
HandleTrack( ITTrackInfo* pTrack )
{
    if ( pTrack == NULL )
        return;

    std::wstring artist = (wchar_t*)pTrack->artist;
	std::wstring albumArtist = (wchar_t*)pTrack->albumArtist;
    std::wstring track = (wchar_t*)pTrack->name;
    artist = Moose::fixStr(artist);
	albumArtist = Moose::fixStr(albumArtist);
    track = Moose::fixStr(track);

    // On a 1x looped track, we will get a ChangeTrack event which means we
    // won't have gone into paused and will just drop through to the new
    // track code.

    bool didWeActuallyResume = false;
    if ( gASState == AS_PAUSED && 
       ( artist == gArtist && track == gTrack ) )
    {
        // We'll get here if the same track started again. If the elapsed
        // time since last stop is less than a second, we interpret it as
        // a restart, if it is longer, we interpret it as a resume.
        DWORD elapsed = GetTickCount() - gTimeOfLastStop;
        didWeActuallyResume = elapsed > 1000;

        LOGL( 3, "Resume check, elapsed: " << elapsed );
    }
        
    if ( didWeActuallyResume )
    {
        ASResume();
    }
    else
    {
        // track change, set globals
        ASStop();

        gArtist  = artist;
		gAlbumArtist = albumArtist;
        gTrack = track;
        gAlbum = (wchar_t*)pTrack->album;
        gAlbum = Moose::fixStr(gAlbum);

        // "Audio CD Track"
        // "MPEG audio stream"
        // "MPEG audio track"...
        gKind = (wchar_t*)pTrack->kind;
        gKind = Moose::fixStr(gKind);

        gLen = pTrack->totalTimeInMS / 1000;
        gStars = pTrack->trackRating;
        gBitrate = pTrack->bitRate;

        std::wstring fileName = (wchar_t*)pTrack->fileName;
        fileName = Moose::fixStr(fileName);

        // What type of track is this?
        char type = '\0';
        if (pTrack->bitRate == 1411)
        {
            // deffo a CD
            type = 'c';
            LOGL( 3, "It's a CD");
        }
        else if (fileName.size() == 0)
        {
            // Filename size is also 0 when playing a file over local network...
            std::transform(gKind.begin(), gKind.end(), gKind.begin(), std::towupper); 
            if (gKind.find(L"STREAM") != std::wstring::npos)
            {
                type = 's';
                LOGL( 3, "It's a stream");
            }
            else
            {
                type = 'f';
                LOGL( 3, "It's a shared playlist file");
            }
        }
        else
        {
            // a music file
            type = 'f';
            LOGL( 3, "It's a file");
        }

        if (type == 's')
        {
            // We don't do streams
            return;
        }

        // Only try and find the path if it's a file as opposed to a stream or
        // a track off CD
        if (type == 'f')
        {
            // Check for video, not the most reliable as it relies on kind string
            //std::string sKind = CW2A(gKind.c_str());
            std::string sKind = "";
            transform(sKind.begin(), sKind.end(), sKind.begin(), tolower);
            if ( (sKind.find("video", 0) != std::string::npos) || 
                (sKind.find("movie", 0) != std::string::npos) )
            {
                LOGL( 3, "Video file, won't submit");
                return;
            }

            if ( fileName.size() > 0 )
            {
                gPath = fileName;
            }
            else
            {
                gPath = L"";
            }
        }
        else if (type == 'c')
        {
            gPath = L"cd";
        }
        else
        {
            gPath = L"";
        }

        LogTrack();

        ASStart();

        // Give this track to the ITunesComThread for syncing playcount with local db
        VisualPluginTrack vpt = {
            gArtist,
			gAlbumArtist,
            gTrack,
            gAlbum,
            gPath,
            pTrack->playCount };
        
        // If iPod scrobbling is switched off, this pointer will not have been
        // initialised so we need to check it first.
        if ( gCom != 0 )
            gCom->syncTrack( vpt );
    }
}


/*******************************************************************************
 * iTunes Plugin callbacks                                                     *
 *******************************************************************************/

static void
ProcessRenderData( VisualPluginData *visualPluginData, const RenderVisualData *renderData )
{
    //  we don't do any complex processing with render data 
    //  at the moment - but it is an indication that we are playing
}


/** DrawVisual - this is the main drawing code */
void DrawVisual( VisualPluginData * visualPluginData )
{
	if ( !(visualPluginData && visualPluginData->destView != NULL) )
        return;

    HDC hdc = GetDC(visualPluginData->destView);

    HBRUSH brush = CreateSolidBrush(RGB(220, 220, 220));

    RECT client;
    ::GetClientRect( visualPluginData->destView, &client );

    // Paint background
    FillRect(hdc, &client, brush);

    // Draw version string
    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT)) ;
    RECT textRect = {10, 10, 400, 30};
    std::ostringstream os;
    os << kTVisualPluginName << " " << GetVersionString();
    DrawText(hdc, os.str().c_str(), -1, &textRect, DT_LEFT | DT_TOP);

    // Draw options box
    RECT boxRect = { visualPluginData->destRect.right - 144, visualPluginData->destRect.top + 10,
                     visualPluginData->destRect.right - 10, visualPluginData->destRect.top + 50 };
    DeleteObject(brush);
    brush = CreateSolidBrush(RGB(250, 250, 250));
    SelectObject(hdc, brush);
    //HPEN pen = GetStockObject(BLACK_PEN);
    SelectObject(hdc, GetStockObject(BLACK_PEN));
    Rectangle(hdc, boxRect.left - 4, boxRect.top - 4, boxRect.right + 4, boxRect.bottom + 4);
    DrawText(hdc, "Open the Last.fm\napplication to change your\nsettings.",
             -1, &boxRect, DT_LEFT | DT_TOP);

    DeleteObject(brush);
    ReleaseDC(visualPluginData->destView, hdc);
}


/** AllocateVisualData is where you should allocate any information that depends
  * on the port or rect changing (like offscreen GWorlds).
  */
static OSStatus
AllocateVisualData (VisualPluginData *visualPluginData, const Rect *destRect)
{
    return noErr;
}

//-------------------------------------------------------------------------------------------------
//	GetVisualOptions
//-------------------------------------------------------------------------------------------------
//
OptionBits GetVisualOptions( void )
{
	return kVisualWantsIdleMessages;
}


/** DeallocateVisualData is where you should deallocate the things you have
  * allocated.
  */
static void
DeallocateVisualData (VisualPluginData *visualPluginData)
{}


static Boolean
RectanglesEqual(const Rect *r1, const Rect *r2)
{
    if (
        (r1->left == r2->left) &&
        (r1->top == r2->top) &&
        (r1->right == r2->right) &&
        (r1->bottom == r2->bottom)
       )
        return true;

    return false;
}


OSStatus MoveVisual( VisualPluginData * visualPluginData, OptionBits newOptions )
{
	GetClientRect( visualPluginData->destView, &visualPluginData->destRect );
	visualPluginData->destOptions = newOptions;

	return noErr;
}


OSStatus DeactivateVisual( VisualPluginData * visualPluginData )
{
	visualPluginData->destView = NULL;
	return noErr;
}


static void
ResetRenderData (VisualPluginData *visualPluginData)
{
    memset( &visualPluginData->renderData, 0, sizeof(visualPluginData->renderData) );
	memset( visualPluginData->minLevel, 0, sizeof(visualPluginData->minLevel) );
}


OSStatus
VisualPluginHandler( OSType message, VisualPluginMessageInfo* messageInfo, void* refCon )
{
    OSStatus status = noErr;

    VisualPluginData* visualPluginData = (VisualPluginData*)refCon;

    #define _( x ) x: LOGL( 3, "EVENT: " #x );

    switch (message)
    {
        /** Sent when the visual plugin is registered.  The plugin should do 
          * minimal memory allocations here.  The resource fork of the plugin is 
          * still available. */
	case _(kVisualPluginInitMessage)
        {
            visualPluginData = (VisualPluginData *)malloc(sizeof(VisualPluginData));
            if (visualPluginData == nil)
            {
                status = memFullErr;
                break;
            }

            visualPluginData->appCookie = messageInfo->u.initMessage.appCookie;
            visualPluginData->appProc   = messageInfo->u.initMessage.appProc;

            // Remember the file spec of our plugin file. We need this so we can
            // open our resource fork during the configuration message

            messageInfo->u.initMessage.refCon = (void*) visualPluginData;
        }
        break;

        /** Sent when the visual plugin is unloaded */
	case _(kVisualPluginCleanupMessage)
        {
            ASStop();

            if (visualPluginData != nil)
                free( visualPluginData );
        }
        break;

        /** Sent when the visual plugin is enabled.  iTunes currently enables
          * all loaded visual plugins.  The plugin should not do anything here.
          */
	case _(kVisualPluginEnableMessage)
            break;

	case _(kVisualPluginDisableMessage)
            ASStop();
            break;

        /** Sent if the plugin requests idle messages.  Do this by setting the
          * kVisualWantsIdleMessages option in the 
          * PlayerRegisterVisualPluginMessage.options field. */
        case kVisualPluginIdleMessage:
        {
            //TODO this is called a lot, lets not do anything here..
            
            // The render msg will take care of refreshing the screen if we're playing
            if ( !visualPluginData->playing )
            {
                DrawVisual( visualPluginData );
            }
        }
        break;

        /** Sent if the plugin requests the ability for the user to configure 
          * it.  Do this by setting the kVisualWantsConfigure option in the 
          * PlayerRegisterVisualPluginMessage.options field. */
		case _(kVisualPluginConfigureMessage)
            break;

        /** Sent when iTunes is going to show the visual plugin in a port.  At
          * this point, the plugin should allocate any large buffers it needs.
          * Gets called once when window is first displayed. */
		case _(kVisualPluginActivateMessage)
        /** Sent when iTunes needs to change the port or rectangle of the
          * currently displayed visual. Resize. */
        case _(kVisualPluginWindowChangedMessage)
        {
 			status = MoveVisual( visualPluginData, messageInfo->u.windowChangedMessage.options );
			break;
        }
        break;

        /** Sent when iTunes is no longer displayed. */
		case _(kVisualPluginDeactivateMessage)
        {
            status = DeactivateVisual( visualPluginData );
        }
        break;

        /** Sent for the visual plugin to render a frame. Only get these when 
          * playing. */
        case _(kVisualPluginDrawMessage)
            DrawVisual( visualPluginData );
            break;

        /** Sent when the playback starts */
		case _(kVisualPluginPlayMessage)
			HandleTrack( messageInfo->u.playMessage.trackInfo );
            visualPluginData->playing = true;
            break;

        /* Sent when the player changes the current track information. This is
         * used when the information about a track changes, or when the CD moves
         * onto the next track. The visual plugin should update any displayed
         * information about the currently playing song. */
		case _(kVisualPluginChangeTrackMessage)
            HandleTrack( messageInfo->u.changeTrackMessage.trackInfo );
            break;

        /** Sent when the player stops. */
		case _(kVisualPluginStopMessage)
        {
            if ( gASState == AS_PLAYING )
            {
                gTimeOfLastStop = GetTickCount();
                ASPause();
            }

            visualPluginData->playing = false;

            ResetRenderData(visualPluginData);
            DrawVisual(visualPluginData);

			if ( gCom )
			{
				// There is not a pause message to we take all
				// stopped messages as pause

				// this will mean we get called back if there is no current track
				// i.e. we have actual stopped (probably at the end of a playlist)
				gCom->callbackIfStopped( StopCallback );

				// This means that the com thread will sync the plays database
				// is an actual stop happened
				gCom->stop();
			}
		}
        break;

        /** Sent when the player changes the track position. */
		case _(kVisualPluginSetPositionMessage)
            break;

        default:
            status = unimpErr;
            break;
    }

    #undef _ //logging helper macro

    return status;
}

void GetVisualName( ITUniStr255 name )
{	
	name[0] = (UniChar)wcslen( kTVisualPluginName );
	wcscpy_s( (wchar_t *)&name[1], 255, kTVisualPluginName );
}
