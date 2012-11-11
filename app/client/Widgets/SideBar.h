/*
   Copyright 2010 Last.fm Ltd.
      - Primarily authored by Jono Cole and Michael Coffey

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

#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>

namespace lastfm { class User; }
namespace unicorn { class Session; }

class SideBar : public QFrame
{
    Q_OBJECT
private:
    struct
    {
        class QAbstractButton* nowPlaying;
        class QAbstractButton* scrobbles;
        class QAbstractButton* profile;
        class QAbstractButton* friends;
        class QAbstractButton* radio;
        class QAbstractButton* sash;
    } ui;

public:
    explicit SideBar(QWidget *parent = 0);

    void addToMenu( class QMenu& menu );
    void click( int index );
    QString currentCategory() const;

signals:
    void currentChanged( int index );

public slots:
    void onButtonClicked( class QAbstractButton* button );

    void onUp();
    void onDown();

private:
    class QButtonGroup* m_buttonGroup;
    class QAbstractButton* m_lastButton;
};

#endif // SIDEBAR_H
