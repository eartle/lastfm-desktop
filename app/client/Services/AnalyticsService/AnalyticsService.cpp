/*
   Copyright 2005-2012 Last.fm Ltd.
      - Primarily authored by Frantz Joseph

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

#include <QString>
#include <QWebView>
#include <QNetworkCookieJar>
#include <QCoreApplication>

#include <lastfm/ws.h>
#include <lastfm/User.h>

#include "lib/unicorn/UnicornSettings.h"

#include "../Application.h"
#include "PersistentCookieJar.h"
#include "AnalyticsService.h"

AnalyticsService::AnalyticsService()
    :m_customVarsSet( false ), m_pageLoaded( false )
{
    m_webView = new QWebView();
    m_cookieJar = new PersistentCookieJar( this );
    m_webView->page()->networkAccessManager()->setCookieJar( m_cookieJar );

    connect( m_webView, SIGNAL(loadFinished(bool)), m_cookieJar, SLOT(save()) );
    connect( m_webView, SIGNAL(loadFinished(bool)), SLOT(onLoadFinished()) );
    connect( aApp, SIGNAL(gotUserInfo(lastfm::User)), SLOT(onGotUserInfo(lastfm::User)) );

    m_webView->load( QString( "http://cdn.last.fm/client/ga.html" ) );
}

void
AnalyticsService::sendEvent( const QString& category, const QString& action, const QString& label, const QString& value )
{
    m_queue.enqueue( QString( "http://cdn.last.fm/client/ga.html#event?category=%1&action=%2&label=%3&value=%4" ).arg( category, action, label, value ) );
    loadPages();
}

void
AnalyticsService::sendPageView( const QString& url )
{
    m_queue.enqueue( QString( "http://cdn.last.fm/client/ga.html#pageview?url=%1" ).arg( url ) );
    loadPages();
}

void
AnalyticsService::loadPages()
{
    if ( m_pageLoaded && m_customVarsSet )
    {
        while ( !m_customVars.isEmpty() )
            m_webView->load( m_customVars.dequeue() );

        while ( !m_queue.isEmpty() )
            m_webView->load( m_queue.dequeue() );
    }
}

void
AnalyticsService::onLoadFinished()
{
    m_pageLoaded = true;
    loadPages();
}

QString userTypeToString( lastfm::User::Type type )
{
    if ( type == lastfm::User::TypeUser ) return "user";
    else if ( type == lastfm::User::TypeSubscriber ) return "subscriber";
    else if ( type == lastfm::User::TypeModerator ) return "moderator";
    else if ( type == lastfm::User::TypeStaff ) return "staff";
    else if ( type == lastfm::User::TypeAlumni ) return "alumni";
    return "unknown";
}

void
AnalyticsService::onGotUserInfo( const lastfm::User& user )
{
    // set all the session level custom vars
    m_customVars.clear();
    m_customVars.enqueue( QString( "http://cdn.last.fm/client/ga.html#custom?version=%1&usertype=%2" ).arg( QCoreApplication::applicationVersion(), userTypeToString( user.type() ) ) );
    m_customVarsSet = true;

    loadPages();
}

