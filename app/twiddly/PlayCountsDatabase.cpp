/*
   Copyright 2005-2009 Last.fm Ltd. 
      - Primarily authored by Max Howell, Jono Cole, Erik Jaelevik, 
        Christian Muehlhaeuser

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
#include "PlayCountsDatabase.h"
#include "TwiddlyApplication.h"
#include "IPod.h"
#include "ITunesLibrary.h"
#include "common/qt/msleep.cpp"
#include "common/c++/fileCreationTime.cpp"
#include "lib/unicorn/UnicornSettings.h"
#include "lib/unicorn/mac/AppleScript.h"
#include <lastfm/misc.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTemporaryFile>
#include <iostream>
#include <QDebug>


/** @author Max Howell <max@last.fm>
  * @brief automatically log sql errors */
namespace QtOverrides
{
    class SqlQuery : public ::QSqlQuery
    {
        // this is called arse because both check and verify wouldn't compile!
        bool arse( bool success )
        {
            if (!success)
                qWarning() << lastError().text() << "in query:\n" << lastQuery();
            return success;
        }
        
    public:
        SqlQuery( QSqlDatabase db ) : QSqlQuery( db )
        {}
                
        bool exec()
        {
            return arse( QSqlQuery::exec() );
        }
        
        bool exec( const QString& sql )
        {
            return arse( QSqlQuery::exec( sql ) );
        }
    };
}

#define QSqlQuery QtOverrides::SqlQuery


PlayCountsDatabase::PlayCountsDatabase( const QString& path )
                   : m_path( path )
{
    qDebug() << path;

    #define TABLE_NAME "itunes_db" //named badly, but too late now
#ifdef Q_OS_MAC
    #define SCHEMA "persistent_id   VARCHAR( 32 ) PRIMARY KEY," \
                   "path            TEXT," \
                   "play_count      INTEGER"

    #define INDEX "persistent_id"
#else
    #define SCHEMA "id              INTEGER PRIMARY KEY AUTOINCREMENT," \
                   "persistent_id   VARCHAR( 32 )," \
                   "path            TEXT UNIQUE," \
                   "play_count      INTEGER"

    #define INDEX "path"
#endif    

    m_db = QSqlDatabase::addDatabase( "QSQLITE", path /*connection-name*/ );
    m_db.setDatabaseName( path );
    m_db.open();

    if ( !m_db.isValid() )
        throw "Could not open " + path;

    QSqlQuery query( m_db );

    if ( !m_db.tables().contains( TABLE_NAME ) )
    {
        query.exec( "CREATE TABLE " TABLE_NAME " ( " SCHEMA " );" );
        query.exec( "CREATE INDEX " INDEX "_idx ON " TABLE_NAME " ( " INDEX " );" );
    }

    m_query = new QSqlQuery( m_db );
#ifdef WIN32
    m_query->prepare( "SELECT play_count FROM itunes_db "
                   "WHERE path = :path LIMIT 1" );
#else
    m_query->prepare( "SELECT play_count FROM itunes_db "
                   "WHERE persistent_id = :pid LIMIT 1" );
#endif

    QSqlQuery snapshotQuery( m_db );
#ifdef WIN32
    snapshotQuery.exec( "SELECT path, play_count FROM itunes_db WHERE path IS NOT '' ORDER BY path ASC" );
#else
    snapshotQuery.exec( "SELECT persistent_id, play_count FROM itunes_db ORDER BY persistent_id ASC" );
#endif

    if ( snapshotQuery.first() )
    {
        do {
            bool ok;
            int count = snapshotQuery.value( 1 ).toInt( &ok );

            if ( ok )
                m_snapshot[snapshotQuery.value(0).toString()] = count;

        } while ( snapshotQuery.next() );
    }
}


PlayCountsDatabase::~PlayCountsDatabase()
{
    // NOTE don't do this! It closes any copies too, but if we let Qt handle it
    // it only closes the connection for the last db instance
    //m_db.close();

    delete m_query;

}


PlayCountsDatabase::Track
PlayCountsDatabase::operator[]( const QString& uid )
{
    if ( m_snapshot.contains( uid ) )
        return Track( uid, m_snapshot[uid] );

    return Track();
}

PlayCountsDatabase::Track
PlayCountsDatabase::track( const QString& uid )
{
#ifdef WIN32
    QString path = uid;
    m_query->bindValue( ":path", path );
#else
    m_query->bindValue( ":pid", uid );
#endif
    m_query->exec();

    Q_ASSERT( m_query->size() < 2 );

    if ( m_query->first() )
    {
        bool ok;
        int count = m_query->value( 0 ).toInt( &ok );
        
        if ( ok )
        {
            return Track( uid, count );
        }
    }

    return Track();
}


void
PlayCountsDatabase::beginTransaction()
{
    qDebug() << "beginTransaction";

    // we try until blue in the face to begin the transaction, as we really,
    // really want db lock
    
    QSqlQuery q( m_db );
    q.exec( "BEGIN TRANSACTION" );
            
    for (int i = 5; q.lastError().type() == QSqlError::ConnectionError && i; i--)
    {
        // we only try 5 times since SQLITE_BUSY is just one of the
        // possible things that ConnectionError might mean
        LOG( 3, "SQLite might be busy trying again in 25ms..." );
        Qt::msleep( 25 );

        q.exec();
    }
}


void
PlayCountsDatabase::endTransaction()
{
    qDebug() << "endTransaction";

    // FIXME: what happens if exec returns false?
    QSqlQuery( m_db ).exec( "END TRANSACTION;" );
}


#ifdef WIN32
bool
PlayCountsDatabase::insert( const ITunesLibrary::Track& track )
{
    QString const sql = "INSERT OR ROLLBACK INTO itunes_db ( persistent_id, path, play_count ) "
                        "VALUES ( :pid, :path, :plays )";
    return exec( sql, track );
}

bool
PlayCountsDatabase::update( const ITunesLibrary::Track& track )
{
    QString const sql = "UPDATE OR ROLLBACK itunes_db "
                        "SET play_count=:plays "
                        "WHERE path=:path";
    return exec( sql, track );
}


bool
PlayCountsDatabase::exec( const QString& sql, const ITunesLibrary::Track& track )
{
    QSqlQuery query( m_db );
    query.prepare( sql );
    // is path for automatic scrobbling, and a nasty hash for manual scrobbling
    query.bindValue( ":path", track.uniqueId() );
    query.bindValue( ":plays", QVariant( (int)track.playCount() ) );
    return query.exec();
}


bool 
PlayCountsDatabase::remove( const ITunesLibraryTrack& track )
{
    QSqlQuery query( m_db );
    query.prepare( "DELETE FROM itunes_db WHERE path=:path" );
    query.bindValue( ":path", track.uniqueId() );
    return query.exec();
}


#else //MAC
bool
PlayCountsDatabase::insert( const ITunesLibrary::Track& track )
{
    QString playCount = QString::number( track.playCount() );  
    QString sql = "INSERT OR ROLLBACK INTO itunes_db ( persistent_id, play_count ) "
                  "VALUES ( '" + track.persistentId() + "', '" + playCount + "' )";
    return QSqlQuery( m_db ).exec( sql );
}

bool
PlayCountsDatabase::update( const ITunesLibrary::Track& track )
{
    QString playCount = QString::number( track.playCount() );  
    QString sql = "UPDATE OR ROLLBACK itunes_db "
                  "SET play_count='" + playCount + "' "
                  "WHERE persistent_id='" + track.persistentId() + "'";
    return QSqlQuery( m_db ).exec( sql );
}
#endif



AutomaticIPod::PlayCountsDatabase::PlayCountsDatabase() 
#ifdef Q_OS_MAC
              : ::PlayCountsDatabase( lastfm::dir::runtimeData().filePath( "iTunesPlays.db" ) )
#else
              : ::PlayCountsDatabase( lastfm::dir::runtimeData().filePath( "Client/iTunesPlays.db" ) )
#endif
{}


bool
AutomaticIPod::PlayCountsDatabase::isBootstrapNeeded() const
{
    QSqlQuery q( m_db );
    q.exec( "SELECT value FROM metadata WHERE key='bootstrap_complete'" );
    if (q.next() && q.value( 0 ).toString() == "true")
        return false;
        
    return true;
}


static QString
pluginPath()
{
  #ifdef Q_OS_MAC
    QString path = std::getenv( "HOME" );
    path += "/Library/iTunes/iTunes Plug-ins/AudioScrobbler.bundle/Contents/MacOS/AudioScrobbler";
    return path;
  #else
    QSettings settings( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Last.fm\\Client\\Plugins\\", QSettings::NativeFormat );
    QString path = settings.value( "itw/Path" ).toString();
    if (path.isEmpty())
        throw "Unknown iTunes plugin path";
    return path;
  #endif
}

void
AutomaticIPod::PlayCountsDatabase::bootstrap()
{
    qDebug() << "Starting bootstrapping...";
    
    static_cast<TwiddlyApplication*>(qApp)->sendBusMessage( "container://Notification/Twiddly/Bootstrap/Started" );

    beginTransaction();    
    
    QSqlQuery query( m_db );
    // this will fail if the metadata table doesn't exist, which is fine
    query.exec( "DELETE FROM metadata WHERE key='bootstrap_complete'" );
    query.exec( "DELETE FROM metadata WHERE key='plugin_ctime'" );
    query.exec( "DELETE FROM itunes_db" );

#ifdef Q_OS_MAC
    ITunesLibrary lib;
    
    // for wizard progress screen
    std::cout << lib.trackCount() << std::endl;
    
    int i = 0;
    while (lib.hasTracks())
    {
        ITunesLibrary::Track const t = lib.nextTrack();
        QString const plays = QString::number( t.playCount() );
        
        query.exec( "INSERT OR IGNORE INTO itunes_db ( persistent_id, play_count ) "
                    "VALUES ( '" + t.uniqueId() + "', '" + plays + "' )" );

        std::cout << ++i << std::endl;
    }

#else

    ITunesLibrary lib;
    int i = 0;
    
    // These cout statements are for the progress indicator in the client,
    // do not remove!
    std::cout << lib.trackCount() << std::endl;
    
    while ( lib.hasTracks() )
    {
        try
        {
            ITunesLibrary::Track t = lib.nextTrack();

            if ( !t.isNull() )
            {
                QString sql = "INSERT OR IGNORE INTO itunes_db ( persistent_id, path, play_count ) "
                              "VALUES ( :pid, :path, :plays )";
                exec( sql, t );
            }
        }
        catch ( ITunesException& )
        {
            // Move on...
        }

        std::cout << ++i << std::endl;
    }

#endif

    // if either INSERTS fail we'll rebootstrap next time
    query.exec( "CREATE TABLE metadata (key VARCHAR( 32 ), value VARCHAR( 32 ))" );
    query.exec( "INSERT INTO metadata (key, value) VALUES ('bootstrap_complete', 'true')" );
    
    QString const t = QString::number( common::fileCreationTime( pluginPath() ) );
    query.exec( "INSERT INTO metadata (key, value) VALUES ('plugin_ctime', '"+t+"')" );


    endTransaction();

    static_cast<TwiddlyApplication*>(qApp)->sendBusMessage( "container://Notification/Twiddly/Bootstrap/Finished" );
}
