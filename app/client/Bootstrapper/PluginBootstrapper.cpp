/*
   Copyright 2005-2009 Last.fm Ltd. 
      - Primarily authored by Max Howell, Jono Cole and Doug Mansell

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

#include <QSettings>
#include <QFile>
#include <QProcess>
#include <QTimer>
#include <QFileDialog>

#include <lastfm/misc.h>
#include <lastfm/ws.h>

#include "lib/unicorn/QMessageBoxBuilder.h"
#include "PluginBootstrapper.h"


PluginBootstrapper::PluginBootstrapper( QString pluginId, QObject* parent )
                   :AbstractBootstrapper( parent ),
                    m_pluginId( pluginId )
{
    connect( this, SIGNAL( done( int ) ), SLOT( onUploadCompleted( int ) ) );
}


void
PluginBootstrapper::bootStrap()
{
    QSettings bootstrap( QSettings::NativeFormat, QSettings::UserScope, "Last.fm", "Bootstrap", this );

    bootstrap.setValue( m_pluginId, lastfm::ws::Username );
    bootstrap.setValue( "data_path", lastfm::dir::runtimeData().path() );

    bootstrap.setValue( "Strings/progress_label",       tr("Last.fm is importing your current media library...") );
    bootstrap.setValue( "Strings/complete_label",       tr("Last.fm has imported your media library.\n\n Click OK to continue.") );
    bootstrap.setValue( "Strings/progress_title",       tr("Last.fm Library Import") );
    bootstrap.setValue( "Strings/cancel_confirmation",  tr("Are you sure you want to cancel the import?") );
    bootstrap.setValue( "Strings/no_tracks_found",      tr("Last.fm couldn't find any played tracks in your media library.\n\n Click OK to continue.") );

    // start the media player
    QProcess* process = new QProcess( this );
    QString mediaPlayer = "";

    if ( m_pluginId == "wa2" )
    {
        mediaPlayer = QString( qgetenv( "ProgramFiles(x86)" ) ).append( "/Winamp/winamp.exe" );
        if ( !QFile::exists( mediaPlayer ) )
            mediaPlayer = QString( qgetenv( "ProgramFiles" ) ).append( "/Winamp/winamp.exe" );
    }
    else
    {
        mediaPlayer = QString( qgetenv( "ProgramFiles(x86)" ) ).append( "/Windows Media Player/wmplayer.exe" );
        if ( !QFile::exists( mediaPlayer ) )
            mediaPlayer = QString( qgetenv( "ProgramFiles" ) ).append( "/Windows Media Player/wmplayer.exe" );
    }

    qDebug() << mediaPlayer;

    if ( !QFile::exists( mediaPlayer ) )
    {
        mediaPlayer = QFileDialog::getOpenFileName( 0,
                                               m_pluginId == "wa2" ? tr( "Where is Winamp?" ) : tr( "Where is Windows Media Player?" ),
                                               QString( getenv( "ProgramFiles(x86)" ) ),
                                               m_pluginId == "wa2" ? "winamp.exe" : "wmplayer.exe" );


    }

    qDebug() << mediaPlayer;

    mediaPlayer = QString( "\"%1\"" ).arg( mediaPlayer );

    if ( !process->startDetached( mediaPlayer ) )
    {
        qDebug() << process->error() << process->errorString();

        // We were unable to start the bootstrap so don't tell the media
        // player to do the bootstrap on it's next launch anymore
        bootstrap.setValue( m_pluginId, "" );
        emit done( Bootstrap_Cancelled );
    }
    else
    {
        // wait for it to do its stuff
        QTimer::singleShot( 1000, this, SLOT(checkBootstrapped()) );
    }
}


void
PluginBootstrapper::checkBootstrapped()
{
    // check if the file exists

    QString savePath = lastfm::dir::runtimeData().filePath( lastfm::ws::Username + "_" + m_pluginId + "_bootstrap.xml" );

    if ( QFile::exists( savePath ) )
    {
        // make sure winamp doesn't create the bootstrap file again
        QSettings bootstrap( QSettings::NativeFormat, QSettings::UserScope, "Last.fm", "Bootstrap", this );
        bootstrap.remove( m_pluginId );

        submitBootstrap();
    }
    else
        QTimer::singleShot( 1000, this, SLOT(checkBootstrapped()) );
}


void
PluginBootstrapper::submitBootstrap()
{
    QString savePath = lastfm::dir::runtimeData().filePath( lastfm::ws::Username + "_" + m_pluginId + "_bootstrap.xml" );
    QString zipPath = savePath + ".gz";

    zipFiles( savePath, zipPath );
    sendZip( zipPath );
}


void
PluginBootstrapper::onUploadCompleted( int status )
{
    QString savePath = lastfm::dir::runtimeData().filePath( lastfm::ws::Username + "_" + m_pluginId + "_bootstrap.xml" );
    QString zipPath = savePath + ".gz";

    if( status == Bootstrap_Ok  )
    {
        QMessageBoxBuilder( 0 )
                .setIcon( QMessageBox::Information )
                .setTitle( tr("Media Library Import Complete") )
                .setText( tr( "Last.fm has submitted your listening history to the server.\n"
                              "Your profile will be updated with the new tracks in a few minutes.") );
    }
    else if( status == Bootstrap_Denied )
    {
        QMessageBoxBuilder( 0 )
            .setIcon( QMessageBox::Warning )
            .setTitle( tr("Library Import Failed") )
            .setText( tr( "Sorry, Last.fm was unable to import your listening history. "
                          "This is probably because you've already scrobbled too many tracks. "
                          "Listening history can only be imported to brand new profiles.") );

        QFile::remove( savePath );
        QFile::remove( zipPath );
    }
}
