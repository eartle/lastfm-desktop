/*
   Copyright 2010 Last.fm Ltd.
      - Primarily authored by Jono Cole and Micahel Coffey

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

#include <QStatusBar>

#include <lastfm/Track.h>
#include <lastfm/User.h>

class QLabel;

namespace Phonon { class VolumeSlider; }
namespace unicorn { class Session; }
namespace unicorn { class Label; }

class StatusBar : public QStatusBar
{
    Q_OBJECT
    public:
        StatusBar( QWidget* parent = 0);

    private slots:
        void onMessagedChanged( const QString& text );
        void setStatus();

        void onConnectionUp();
        void onConnectionDown();

        void onSessionChanged( const unicorn::Session& session );

    private:
        struct
        {
            class QFrame* widget;
            class QPushButton* cog;
            unicorn::Label* message;

            class QFrame* scrobbleWidget;
            unicorn::Label* scrobbleMessage;
            class QLabel* scrobbleIcon;
        } ui;

        bool m_online;
};
