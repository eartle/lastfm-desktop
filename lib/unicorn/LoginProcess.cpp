#include "LoginProcess.h"
#include "QMessageBoxBuilder.h"
#include "UnicornApplication.h"

#include <lastfm/ws.h>
#include <lastfm/misc.h>
#include <lastfm/XmlQuery.h>

#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QTcpServer>
#include <QTcpSocket>

#include "lib/unicorn/DesktopServices.h"

#ifdef WIN32
#include <windows.h>
#endif

namespace unicorn
{

LoginProcess::LoginProcess( QObject* parent )
    : QObject( parent )
{
}

LoginProcess::~LoginProcess()
{
}

void
LoginProcess::authenticate()
{
    connect( unicorn::Session::getToken(), SIGNAL( finished() ), SLOT( onGotToken() ) );
}

void
LoginProcess::onGotToken()
{
    lastfm::XmlQuery lfm;

    QUrl authUrl;

    if ( lfm.parse( static_cast<QNetworkReply*>( sender() ) ) )
    {
        m_token = lfm["token"].text();

        authUrl.setUrl( "http://www.last.fm/api/auth/" );
        authUrl.addQueryItem( "api_key", lastfm::ws::ApiKey );
        authUrl.addQueryItem( "token", m_token );
        QDesktopServices::openUrl( authUrl );
    }
    else
    {
        handleError( lfm );
    }


    emit authUrlChanged( authUrl.toString() );
}


void
LoginProcess::getSession()
{
    connect( unicorn::Session::getSession( m_token ), SIGNAL( finished() ), SLOT( onGotSession() ) );
}


void
LoginProcess::onGotSession()
{
    lastfm::XmlQuery lfm;

    if ( lfm.parse( static_cast<QNetworkReply*>( sender() ) ) )
    {
        QString username = lfm["session"]["name"].text();
        QString sessionKey = lfm["session"]["key"].text();

        unicorn::Application* app = qobject_cast<unicorn::Application*>( qApp );
        app->changeSession( username, sessionKey );
    }
    else
    {
        handleError( lfm );
    }
}

void
LoginProcess::handleError( const lastfm::XmlQuery& lfm )
{
    qWarning() << lfm.parseError().message() << lfm.parseError().enumValue();

    QString errorText;

    if ( lfm.parseError().enumValue() == lastfm::ws::UnknownError )
        errorText = tr( "There was a network error: %1" ).arg( QString::number( static_cast<QNetworkReply*>( sender() )->error() ) );
    else if ( lfm.parseError().enumValue() == lastfm::ws::TokenNotAuthorised )
        errorText = tr( "You have not authorised this application" );
    else
        errorText = lfm.parseError().message().trimmed() + ": " + QString::number( lfm.parseError().enumValue() );

    QMessageBoxBuilder( 0 )
            .setIcon( QMessageBox::Critical )
            .setTitle( tr("Authentication Error") )
            .setText( errorText )
            .exec();
}

}// namespace unicorn
