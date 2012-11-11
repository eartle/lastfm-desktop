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

#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QShortcut>
#include <QMenu>

#include <lastfm/User.h>

#include "lib/unicorn/widgets/AvatarWidget.h"
#include "../Services/AnalyticsService.h"

#include "../Application.h"
#include "SideBar.h"

QAbstractButton* newButton( const QString& text, QButtonGroup* buttonGroup, QWidget* parent = 0 )
{
    QAbstractButton* pushButton = new QPushButton( parent );
    pushButton->setText( text );
    pushButton->setCheckable( true );
    pushButton->setAutoExclusive( true );
    pushButton->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
    pushButton->setAttribute( Qt::WA_LayoutUsesWidgetRect );
    pushButton->setAttribute( Qt::WA_MacNoClickThrough );
    buttonGroup->addButton( pushButton );
    return pushButton;
}


SideBar::SideBar(QWidget *parent)
    :QFrame(parent), m_lastButton( 0 )
{
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );

    m_buttonGroup = new QButtonGroup( this );
    m_buttonGroup->setExclusive( true );

    layout->addWidget( ui.nowPlaying = newButton( tr( "Now Playing" ), m_buttonGroup, this ), Qt::AlignHCenter );
    ui.nowPlaying->setObjectName( "nowPlaying" );
    layout->addWidget( ui.scrobbles = newButton( tr( "Scrobbles" ), m_buttonGroup, this ), Qt::AlignHCenter);
    ui.scrobbles->setObjectName( "scrobbles" ); 
    layout->addWidget( ui.profile = newButton( tr( "Profile" ), m_buttonGroup, this ), Qt::AlignHCenter);
    ui.profile->setObjectName( "profile" );
    layout->addWidget( ui.friends = newButton( tr( "Friends" ), m_buttonGroup, this ), Qt::AlignHCenter);
    ui.friends->setObjectName( "friends" );
    layout->addWidget( ui.radio = newButton( tr( "Radio" ), m_buttonGroup, this ), Qt::AlignHCenter);
    ui.radio->setObjectName( "radio" );
    layout->addStretch( 1 );

    layout->addWidget( ui.sash = new QPushButton( tr( "Sash" ), this ), Qt::AlignLeft | Qt::AlignBottom );
    ui.sash->setObjectName( "sash" );

    connect( m_buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), SLOT(onButtonClicked(QAbstractButton*)));
    connect( ui.scrobbles, SIGNAL(clicked()), SLOT(onButtonClicked()));
    connect( ui.profile, SIGNAL(clicked()), SLOT(onButtonClicked()));
    connect( ui.friends, SIGNAL(clicked()), SLOT(onButtonClicked()));
    connect( ui.radio, SIGNAL(clicked()), SLOT(onButtonClicked()));

    connect( ui.sash, SIGNAL(clicked()), aApp, SLOT(onBetaTriggered()));

    ui.nowPlaying->click();
}

QString
SideBar::currentCategory() const
{
    if ( m_buttonGroup->checkedButton() == ui.nowPlaying ) return NOW_PLAYING_CATEGORY;
    else if ( m_buttonGroup->checkedButton() == ui.scrobbles ) return SCROBBLES_CATEGORY;
    else if ( m_buttonGroup->checkedButton() == ui.profile ) return PROFILE_CATEGORY;
    else if ( m_buttonGroup->checkedButton() == ui.friends ) return FRIENDS_CATEGORY;
    else if ( m_buttonGroup->checkedButton() == ui.radio ) return RADIO_CATEGORY;
    return "Unknown";
}

void
SideBar::addToMenu( QMenu& menu )
{
    menu.addAction( ui.nowPlaying->text(), ui.nowPlaying, SLOT(click()), Qt::CTRL + Qt::Key_1);
    menu.addAction( ui.scrobbles->text(), ui.scrobbles, SLOT(click()), Qt::CTRL + Qt::Key_2);
    menu.addAction( ui.profile->text(), ui.profile, SLOT(click()), Qt::CTRL + Qt::Key_3);
    menu.addAction( ui.friends->text(), ui.friends, SLOT(click()), Qt::CTRL + Qt::Key_4);
    menu.addAction( ui.radio->text(), ui.radio, SLOT(click()), Qt::CTRL + Qt::Key_5);

    menu.addSeparator();

    menu.addAction( "Next Section", this, SLOT(onDown()), Qt::CTRL + Qt::Key_BracketRight);
    menu.addAction( "Previous Section", this, SLOT(onUp()), Qt::CTRL + Qt::Key_BracketLeft);
}

void
SideBar::onUp()
{
    if ( ui.nowPlaying->isChecked() ) ui.radio->click();
    else if ( ui.scrobbles->isChecked() ) ui.nowPlaying->click();
    else if ( ui.profile->isChecked() ) ui.scrobbles->click();
    else if ( ui.friends->isChecked() ) ui.profile->click();
    else if ( ui.radio->isChecked() ) ui.friends->click();
}

void
SideBar::onDown()
{
    if ( ui.nowPlaying->isChecked() ) ui.scrobbles->click();
    else if ( ui.scrobbles->isChecked() ) ui.profile->click();
    else if ( ui.profile->isChecked() ) ui.friends->click();
    else if ( ui.friends->isChecked() ) ui.radio->click();
    else if ( ui.radio->isChecked() ) ui.nowPlaying->click();
}

void
SideBar::click( int index )
{
    qobject_cast<QAbstractButton*>( layout()->itemAt( index )->widget() )->click();
}


void
SideBar::onButtonClicked( QAbstractButton* button )
{
    int index = layout()->indexOf( button );

    if ( button != m_lastButton )
    {
        if ( button == ui.nowPlaying ) AnalyticsService::instance().sendPageView( NOW_PLAYING_CATEGORY );
        // the scrobble tab is a bit more complicataed so it sends its own for now
        //else if ( button == ui.scrobbles ) AnalyticsService::instance().sendPageView( SCROBBLES_CATEGORY );
        else if ( button == ui.profile ) AnalyticsService::instance().sendPageView( PROFILE_CATEGORY );
        else if ( button == ui.friends ) AnalyticsService::instance().sendPageView( FRIENDS_CATEGORY );
        else if ( button == ui.radio ) AnalyticsService::instance().sendPageView( RADIO_CATEGORY );
    }

    m_lastButton = button;

    emit currentChanged( index );
}
