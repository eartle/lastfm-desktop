/***************************************************************************
 *   Copyright 2005-2008 Last.fm Ltd.                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *    This program is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "RadioWidget.h"
#include "NowPlayingTuner.h"
#include "lib/radio/RadioController.h"
#include "lib/unicorn/widgets/SpinnerLabel.h"
#include "MyStations.h"
#include <QAction>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMovie>
#include <QToolBar>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTabWidget>

RadioWidget::RadioWidget( QWidget* parent )
           : QWidget( parent )
{
	setLayout( new QHBoxLayout );
	
    qRegisterMetaType<Track>( "Track" );

	
	QTabWidget* tabWidget = new QTabWidget;
	NowPlayingTuner* nowPlaying = new NowPlayingTuner;
	tabWidget->addTab( nowPlaying, "Now Playing" );
	connect( nowPlaying, SIGNAL( tune( const RadioStation&)), SLOT( onTune( const RadioStation&)));

	QSplitter* s = new QSplitter( Qt::Vertical, this );
	s->addWidget( tabWidget );
	s->addWidget( new MyStations );
	
	//FIXME: should not rely on hard coded sizes
	//		 but I can't be dealing with getting
	//		 sizing policies working right now!
	s->setSizes( QList<int>() << 373 << 98 );

	layout()->addWidget( s );
	
    setWindowTitle( tr("Last.fm Radio") );

}


void
RadioWidget::onTune( const RadioStation& r )
{
    m_radioController->play( r );
	emit newStationStarted();
}


void
RadioWidget::setRadioController( RadioController* r )
{
	m_radioController = r;
}