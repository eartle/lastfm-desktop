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
#pragma once

#include <QObject>
#include <QQueue>

namespace lastfm { class User; }

// -- Categories and their associated actions
#define NOW_PLAYING_CATEGORY "NowPlaying"
#define SCROBBLES_CATEGORY "Scrobbles"
#define PROFILE_CATEGORY "Profile"
#define FRIENDS_CATEGORY "Friends"
#define RADIO_CATEGORY "Radio"

#define SETTINGS_CATEGORY "Settings"

#define LOVE_TRACK "LoveTrack"
#define BAN_TRACK "BanTrack"
#define LINK_CLICKED "LinkClicked"
#define PLAY_CLICKED "PlayClicked"
#define PLAY_NEXT_CLICKED "PlayNextClicked"
#define PLAY_MULTI_CLICKED "PlayMultiClicked"
#define PLAY_MULTI_NEXT_CLICKED "PlayMultiNextClicked"
#define QUICKSTART_PLAY_CLICKED "QuickstartPlayClicked"
#define QUICKSTART_PLAY_NEXT_CLICKED "QuickstartPlayNextClicked"
#define SHARE_CLICKED "ShareClicked"
#define TAG_CLICKED "TagClicked"
#define SIDE_BAR_CLICKED "SideBarClicked"
#define SKIP_CLICKED "SkipClicked"
#define BUY_CLICKED "BuyClicked"

#define SETTINGS_CLICKED "SettingsClicked"
#define BASIC_SETTINGS "Basic"
#define SCROBBLING_SETTINGS "Scrobbling"

class AnalyticsService : public QObject
{
    Q_OBJECT

public:
    AnalyticsService();

    static AnalyticsService& instance(){ static AnalyticsService a; return a; }

public:
    void sendEvent( const QString& category, const QString& action, const QString& label, const QString& value = "" );
    void sendPageView( const QString& url );

private:
    void loadPages();

private slots:
    void onGotUserInfo( const lastfm::User& user );
    void onLoadFinished();

private:
    class QWebView* m_webView;
    class PersistentCookieJar* m_cookieJar;
    QQueue<QString> m_queue;
    QQueue<QString> m_customVars;
    bool m_customVarsSet;
    bool m_pageLoaded;
};
