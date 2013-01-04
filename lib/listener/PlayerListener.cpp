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
#include "PlayerListener.h"
#include "PlayerCommandParser.h"
#include "PlayerConnection.h"
#include <QLocalSocket>
#include <QDir>
#include <QFile>
#include <QThread>

#ifdef Q_OS_WIN
#include "win/NamedPipeServer.h"
#else
#include <lastfm/misc.h>
#endif

PlayerListener::PlayerListener( QObject* parent )
              : QLocalServer( parent )
{
    connect( this, SIGNAL(newConnection()), SLOT(onNewConnection()) );

    // Create a user-unique name to listen on.
    // User-unique so that different logged-on users 
    // can run their own scrobbler instances.

#ifdef Q_OS_WIN

    NamedPipeServer* namedPipeServer = new NamedPipeServer( this );
    connect( namedPipeServer, SIGNAL(lineReady(QString)), this, SLOT(processLine(QString)), Qt::BlockingQueuedConnection );
    namedPipeServer->start();

#else
    QString const name = "lastfm_scrobsub";

    // on windows we use named pipes which auto-delete
    // *nix platforms need more help:

    QString fullPath = lastfm::dir::runtimeData().absolutePath() + "/" + name;
    if( QFile::exists( fullPath ))
        QFile::remove( fullPath );

    bool success = listen( fullPath );
    Q_ASSERT( success );
#endif
}

void
PlayerListener::onNewConnection()
{
    qDebug() << hasPendingConnections();

    while (hasPendingConnections())
    {
        QObject* o = nextPendingConnection();
        connect( o, SIGNAL(readyRead()), SLOT(onDataReady()) );
        connect( o, SIGNAL(disconnected()), o, SLOT(deleteLater()) );
    }
}

void
PlayerListener::onDataReady()
{
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    if (!socket) return;

    while (socket->canReadLine())
    {
        QString const line = QString::fromUtf8( socket->readLine() );
        QString response = processLine( line );
        socket->write( response.toUtf8() );
    }
}

QString
PlayerListener::processLine( const QString& line )
{
    QString response = "OK\n";

    try
    {
        PlayerCommandParser parser( line );
        QString const id = parser.playerId();
        PlayerConnection* connection = 0;

        if (!m_connections.contains( id ))
        {
            connection = m_connections[id] = new PlayerConnection( parser.playerId(), parser.playerName() );
            emit newConnection( connection );
        }
        else
            connection = m_connections[id];

        switch (parser.command())
        {
            case CommandBootstrap:
                emit bootstrapCompleted( parser.playerId() );
                break;

            case CommandTerm:
                delete connection;
                m_connections.remove( parser.playerId() );
                break;

            default:
                connection->handleCommand( parser.command(), parser.track() );
                break;
        }
    }
    catch (std::invalid_argument& e)
    {
        qWarning() << e.what();
        response = "ERROR: " + QString::fromStdString(e.what()) + "\n";
    }

    return response;
}
