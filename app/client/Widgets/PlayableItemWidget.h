/*
   Copyright 2005-2009 Last.fm Ltd. 

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

#ifndef PLAYABLE_ITEM_WIDGET_H
#define PLAYABLE_ITEM_WIDGET_H

#include <QPushButton>
#include <lastfm/RadioStation.h>

class QMouseEvent;

namespace unicorn { class Session; }

class PlayableItemWidget : public QPushButton
{
    Q_OBJECT

public:
    enum Style
    {
        DescriptionElide,
        DescriptionBottom,
        ThreePart,
        DescriptionNone
    };

    Q_ENUMS( Style )
    Q_PROPERTY( Style style READ style WRITE setStyle )

public:
    PlayableItemWidget( QWidget* parent = 0 );
    PlayableItemWidget( const RadioStation& rs, const QString& title, const QString& description = "", QWidget* parent = 0 );

    void setStation( const RadioStation& rs, const QString& title, const QString& description = "" );
    void setDescription( const QString& description );

    const RadioStation& station() const { return m_rs; }

    Style style() const { return m_style; }
    void setStyle( Style style ) { m_style = style; }

public slots:
    void onRadioChanged();
    void onRadioChanged( const RadioStation& station );

    void play();
    void playNext();
    void playMulti();
    void playMultiNext();

private slots:
    void onSessionChanged( const unicorn::Session& session );

private:
    RadioStation getMultiStation() const;

    void paintEvent( QPaintEvent* event );
    void contextMenuEvent( class QContextMenuEvent* event );

    bool event( QEvent* e );

    QSize sizeHint() const;

private:
    RadioStation m_rs;
    QString m_description;

    bool m_hovered;

    Style m_style;
};

#endif
