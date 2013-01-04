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

#include <QPainter>
#include <QPaintEvent>
#include <QLayout>
#include <QLabel>
#include <QStyle>
#include <QFont>
#include <QMenu>

#include <lastfm/RadioStation.h>

#include "PlayableItemWidget.h"
#include "../Application.h"
#include "../Services/RadioService.h"
#include "../Services/AnalyticsService.h"

PlayableItemWidget::PlayableItemWidget( QWidget* parent )
    : QPushButton( parent ),
      m_hovered( false ),
      m_style( DescriptionBottom )
{
    setAttribute( Qt::WA_LayoutUsesWidgetRect );
    setAttribute( Qt::WA_Hover );
    setAttribute( Qt::WA_MacNoClickThrough );

    setCursor( Qt::PointingHandCursor );

    connect( aApp, SIGNAL(sessionChanged(unicorn::Session)), SLOT(onSessionChanged(unicorn::Session)) );
}

PlayableItemWidget::PlayableItemWidget( const RadioStation& rs, const QString& title, const QString& description, QWidget* parent )
    : QPushButton( parent ),
      m_rs(rs),
      m_description( description ),
      m_hovered( false ),
      m_style( DescriptionBottom )
{
    setStation( rs, title, description );

    setAttribute( Qt::WA_LayoutUsesWidgetRect );
    setAttribute( Qt::WA_Hover );
    setAttribute( Qt::WA_MacNoClickThrough );

    setCursor( Qt::PointingHandCursor );

    connect( &RadioService::instance(), SIGNAL(tuningIn(RadioStation)), SLOT(onRadioChanged(RadioStation)) );
    connect( &RadioService::instance(), SIGNAL(trackSpooled(Track)), SLOT(onRadioChanged()));

    connect( aApp, SIGNAL(sessionChanged(unicorn::Session)), SLOT(onSessionChanged(unicorn::Session)) );
}

void
PlayableItemWidget::onSessionChanged( const unicorn::Session& session )
{
    setVisible( session.youRadio() );
}

bool
PlayableItemWidget::event( QEvent* e )
{
    switch ( e->type() )
    {
    case QEvent::HoverEnter:
        m_hovered = true;
        update();
        break;
    case QEvent::HoverLeave:
        m_hovered = false;
        update();
        break;
    case QEvent::Show:
        onSessionChanged( aApp->currentSession() );
        break;
    default:
        break;
    }

    return QPushButton::event( e );
}

QSize
PlayableItemWidget::sizeHint() const
{
    QSize sizeHint = QPushButton::sizeHint();

    if ( m_style == DescriptionElide )
        sizeHint.setWidth( 0 );

    return sizeHint;
}

void
PlayableItemWidget::setStation(const RadioStation& rs, const QString& title, const QString& description)
{
    // disconnect from recieving any previous signals
    disconnect( this, 0 );

    m_rs = rs;
    m_rs.setTitle( title );
    setText( title.isEmpty() ? tr( "A Radio Station" ) :  title );
    setToolTip( tr( "Play %1" ).arg( text() ) );

    m_description = description;

    connect( &RadioService::instance(), SIGNAL(tuningIn(RadioStation)), SLOT(onRadioChanged(RadioStation)) );
    connect( &RadioService::instance(), SIGNAL(trackSpooled(Track)), SLOT(onRadioChanged()));

    connect( this, SIGNAL(clicked()), SLOT(play()));

    update();
}

void
PlayableItemWidget::setDescription( const QString& description )
{
    m_description = description;
    update();
}

void 
PlayableItemWidget::play()
{
    AnalyticsService::instance().sendEvent( aApp->currentCategory(), PLAY_CLICKED, objectName() );
    RadioService::instance().play( m_rs );
}

void
PlayableItemWidget::playNext()
{
    AnalyticsService::instance().sendEvent( aApp->currentCategory(), PLAY_NEXT_CLICKED, objectName() );
    RadioService::instance().playNext( m_rs );
}

RadioStation
PlayableItemWidget::getMultiStation() const
{
    QList<User> users;

    int endPos = m_rs.url().indexOf( "/", 14 );
    if ( endPos == -1 )
        endPos = m_rs.url().length();

    users << User( m_rs.url().mid( 14, endPos - 14 ) );
    users << User();

    RadioStation station = RadioStation::library( users );
    station.setTitle( tr( "Multi-Library Radio" ) );
    return station;
}

void
PlayableItemWidget::playMulti()
{
    if ( m_rs.url().startsWith("lastfm://user/") )
    {
        AnalyticsService::instance().sendEvent( aApp->currentCategory(), PLAY_MULTI_CLICKED, objectName());
        RadioService::instance().play( getMultiStation() );
    }
}

void
PlayableItemWidget::playMultiNext()
{
    if ( m_rs.url().startsWith("lastfm://user/") )
    {
        AnalyticsService::instance().sendEvent( aApp->currentCategory(), PLAY_MULTI_NEXT_CLICKED, objectName());
        RadioService::instance().playNext( getMultiStation() );
    }
}

void
PlayableItemWidget::onRadioChanged()
{
    onRadioChanged( RadioService::instance().station() );
}


void
PlayableItemWidget::onRadioChanged( const RadioStation& station )
{
    if ( station == m_rs )
    {
        // This is the current radio station
        if ( !station.title().isEmpty() )
            setText( station.title() );
    }
    else
    {
        // this is not the current radio station
    }
}

void
PlayableItemWidget::paintEvent( QPaintEvent* event )
{   
    static QPixmap m_radio_left_hover = QPixmap( ":/meta_radio_LEFT_HOVER.png" );
    static QPixmap m_radio_left_press = QPixmap( ":/meta_radio_LEFT_PRESS.png" );
    static QPixmap m_radio_left_rest = QPixmap( ":/meta_radio_LEFT_REST.png" );

    static QPixmap m_radio_middle_hover = QPixmap( ":/meta_radio_MIDDLE_HOVER.png" );
    static QPixmap m_radio_middle_press = QPixmap( ":/meta_radio_MIDDLE_PRESS.png" );
    static QPixmap m_radio_middle_rest = QPixmap( ":/meta_radio_MIDDLE_REST.png" );

    static QPixmap m_radio_right_hover = QPixmap( ":/meta_radio_RIGHT_HOVER.png" );
    static QPixmap m_radio_right_press = QPixmap( ":/meta_radio_RIGHT_PRESS.png" );
    static QPixmap m_radio_right_rest = QPixmap( ":/meta_radio_RIGHT_REST.png" );

    static QPixmap m_radio_small_hover = QPixmap( ":/radio_play_small_HOVER.png" );
    static QPixmap m_radio_small_press = QPixmap( ":/radio_play_small_PRESS.png" );
    static QPixmap m_radio_small_rest = QPixmap( ":/radio_play_small_REST.png" );

    if ( m_style == DescriptionNone )
    {
        QPushButton::paintEvent( event );
    }
    else if ( m_style == DescriptionElide )
    {
        QPainter p(this);

        QTextOption to;
        to.setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

        QRect textRect = contentsRect().adjusted( 30, 0, 0, 0 );
        p.drawText( textRect, fontMetrics().elidedText( text(), Qt::ElideRight, textRect.width() ), to );

        if ( isDown() )
            p.drawPixmap( rect().topLeft(), m_radio_small_press );
        else if ( m_hovered )
            p.drawPixmap( rect().topLeft(), m_radio_small_hover );
        else
            p.drawPixmap( rect().topLeft(), m_radio_small_rest );
    }
    else if ( m_style == DescriptionBottom )
    {
        QPushButton::paintEvent( event );

        QPainter p( this );

        p.setPen( QColor( 0x898989 ) );

        QFont font = p.font();
        font.setPixelSize( 12 );
        p.setFont( font );

        QTextOption to;
        to.setAlignment( Qt::AlignBottom );

        QFontMetrics fm( font );

        QRect rect = contentsRect();
        rect.adjust( 54, 0, 0, -14 );
        p.drawText( rect, fm.elidedText( m_description, Qt::ElideRight, rect.width() ), to );
    }
    else if ( m_style == ThreePart )
    {
        QPainter p( this );

        QRect middleRect = QRect( m_radio_left_rest.width(), 0, rect().width() - m_radio_left_rest.width() - m_radio_right_rest.width(), 49 );

        if ( isDown() )
        {
            p.drawPixmap( rect().topLeft(), m_radio_left_press );
            p.drawPixmap( middleRect, m_radio_middle_press );
            p.drawPixmap( rect().topLeft() + QPoint( rect().width() - m_radio_right_press.width(), 0 ), m_radio_right_press );
        }
        else if ( m_hovered )
        {
            p.drawPixmap( rect().topLeft(), m_radio_left_hover );
            p.drawPixmap( middleRect, m_radio_middle_hover );
            p.drawPixmap( rect().topLeft() + QPoint( rect().width() - m_radio_right_hover.width(), 0 ), m_radio_right_hover );
        }
        else
        {
            p.drawPixmap( rect().topLeft(), m_radio_left_rest );
            p.drawPixmap( middleRect, m_radio_middle_rest );
            p.drawPixmap( rect().topLeft() + QPoint( rect().width() - m_radio_right_rest.width(), 0 ), m_radio_right_rest );
        }

        QFontMetrics fmText( font() );
        QRect textRect = rect().adjusted( 40, 8, -20, 0 );
        p.drawText( textRect, fmText.elidedText( text(), Qt::ElideRight, textRect.width() ) );

        p.setPen( QColor( 0x898989 ) );

        QFont font = p.font();
        font.setPixelSize( 10 );
        p.setFont( font );

        QFontMetrics fmDesc( font );
        QRect descRect = rect().adjusted( 40, 28, -20, 0 );
        p.drawText( descRect, fmDesc.elidedText( m_description, Qt::ElideRight, descRect.width() ) );
    }
}

void
PlayableItemWidget::contextMenuEvent( QContextMenuEvent* event )
{
    QMenu* contextMenu = new QMenu( this );

    contextMenu->addAction( tr( "Play %1" ).arg( m_rs.title() ), this, SLOT(play()));

    if ( RadioService::instance().state() == Playing )
        contextMenu->addAction( tr( "Cue %1" ).arg( m_rs.title() ), this, SLOT(playNext()));

    if ( m_rs.url().startsWith( "lastfm://user/" )
         &&  ( m_rs.url().endsWith( "/library" ) || m_rs.url().endsWith( "/personal" ) )
         && m_rs.url() != RadioStation::library( User() ).url() )
    {
        int endPos = m_rs.url().indexOf( "/", 14 );
        if ( endPos == -1 )
            endPos = m_rs.url().length();

        // if it's a user station that isn't yours we should
        // let them start a multi-station with yours
        contextMenu->addSeparator();
        contextMenu->addAction( tr( "Play %1 and %2 Library Radio" ).arg( m_rs.url().mid( 14, endPos - 14 ), User().name() ), this, SLOT(playMulti()));
        if ( RadioService::instance().state() == Playing )
            contextMenu->addAction( tr( "Cue %1 and %2 Library Radio" ).arg( m_rs.url().mid( 14, endPos - 14 ), User().name() ), this, SLOT(playMultiNext()));
    }

    if ( contextMenu->actions().count() )
        contextMenu->exec( event->globalPos() );
}
