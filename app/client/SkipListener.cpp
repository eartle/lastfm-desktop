#include <QTcpServer>
#include <QTcpSocket>
#include <qmath.h>

#include "SkipListener.h"

#include "Services/RadioService.h"

SkipListener::SkipListener(QObject *parent)
    :QObject(parent)
{
    m_server = new QTcpServer( this );

    bool success = m_server->listen( QHostAddress::Any, 54321 );

    if ( !success )
        qDebug() << m_server->errorString();

    connect( m_server, SIGNAL(newConnection()), SLOT(onNewConnection()));

    connect( &RadioService::instance(), SIGNAL(trackSpooled(Track)), SLOT(onTrackSpooled()));
}


void
SkipListener::onTrackSpooled()
{
    m_skippers.clear();
}

QStringList
SkipListener::users( const lastfm::RadioStation& rs )
{
    QStringList users;

    if ( rs.url().startsWith( "lastfm://user/" ) )
    {
        int endPos = rs.url().indexOf( "/", 14 );
        if ( endPos == -1 )
            endPos = rs.url().length();

        users << rs.url().mid( 14, endPos - 14 );
    }
    else if ( rs.url().startsWith( "lastfm://users/" ) )
    {
        int endPos = rs.url().indexOf( "/", 15 );
        if ( endPos == -1 )
            endPos = rs.url().length();

        users << rs.url().mid( 15, endPos - 15 ).split( "," );
    }

    return users;
}

void
SkipListener::onNewConnection()
{
    QTcpSocket* rx = m_server->nextPendingConnection();

    rx->waitForReadyRead();

    QStringList data = QString( rx->readAll() ).split( " ", QString::SkipEmptyParts );

    for ( int i = 0 ; i < data.count() ; ++i )
        data[i] = data[i].trimmed().toLower();

    qDebug() << data;

    QString message = "I beg your pardon";

    if ( data.count() > 1 )
    {
        if ( data[1] == "skip" )
        {
            if ( m_skippers.contains( data[0] ) )
                message = QString( "%1: you've already voted" ).arg( data[0] );
            else
            {
                m_skippers.append( data[0] );

                int totalUsers = RadioService::instance().station().url().count(",") + 1;

                int skipThreshold = qCeil( totalUsers / 2.0f );

                if ( m_skippers.count()  >= skipThreshold )
                {
                    RadioService::instance().skip();
                    message = QString( "Skip!" );
                }
                else
                    message = QString( "%1 of %2 skips needed" ).arg( QString::number( m_skippers.count() ), QString::number( skipThreshold ) );
            }
        }
        else if ( data[1] == "add" )
        {
            QStringList newUsers = data.mid( 2 );
            QStringList currentUsers = users( RadioService::instance().station().url() );

            if ( newUsers.count() > 0 && currentUsers.count() > 0 )
            {
                QList<lastfm::User> users;

                // add all the users
                QStringList allUsers = currentUsers + newUsers;
                allUsers.removeDuplicates();
                foreach ( const QString& user, allUsers )
                    users << lastfm::User( user );

                // retune the station
                lastfm::RadioStation station = lastfm::RadioStation::library( users );
                RadioService::instance().playNext( station );

                message = QString( "Retuning to %1" ).arg( station.url() );
            }
        }
        else if ( data[1] == "remove"
                  || data[1] == "rm" )
        {
            QStringList rmUsers = data.mid( 2 );
            QStringList currentUsers = users( RadioService::instance().station().url() );

            if ( currentUsers.count() > 0 )
            {
                // remove the rm users
                foreach( const QString& rmUser, rmUsers )
                    currentUsers.removeAll( rmUser );

                if ( currentUsers.count() > 0 )
                {
                    QList<lastfm::User> users;

                    // add the user
                    foreach ( const QString& currentUser, currentUsers )
                        users << lastfm::User( currentUser );

                    // retune the station
                    lastfm::RadioStation station = lastfm::RadioStation::library( users );
                    RadioService::instance().playNext( station );

                    message = QString( "Retuning to %1" ).arg( station.url() );
                }
                else
                {
                    RadioService::instance().stop();
                    message = QString( "All users removed" );
                }
            }
        }
        else if ( data[1] == "start" )
        {
            if( RadioService::instance().state() != Stopped )
                message = QString( "Already started" );
            else
            {
                QStringList userList = data.mid( 2 );

                if ( userList.count() > 0 )
                {
                    QList<lastfm::User> users;

                    userList.removeDuplicates();

                    foreach ( const QString& user, userList )
                        users << lastfm::User( user );

                    lastfm::RadioStation station = lastfm::RadioStation::library( users );

                    RadioService::instance().play( station );

                    message = QString( "Starting %1" ).arg( station.url() );
                }
            }
        }
    }

    rx->close();

    // send the message back
    QString ircMessage = QString( "#last.clientroomradio %1" ).arg( message );

    QTcpSocket tx;
    tx.connectToHost( "localhost", 12345 );
    tx.waitForConnected();
    tx.write( ircMessage.toUtf8() );
    tx.flush();
    tx.close();
}


