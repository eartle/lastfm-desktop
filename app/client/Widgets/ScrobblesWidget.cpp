
#include <QWidget>
#include <QLayoutItem>
#include <QDebug>

#include <lastfm/Track.h>

#include "lib/unicorn/layouts/SideBySideLayout.h"

#include "../Services/AnalyticsService/AnalyticsService.h"
#include "ScrobblesListWidget.h"
#include "TrackWidget.h"
#include "MetadataWidget.h"
#include "ScrobblesWidget.h"
#include "ui_ScrobblesWidget.h"

ScrobblesWidget::ScrobblesWidget( QWidget* parent )
    :QWidget( parent ), ui( new Ui::ScrobblesWidget ), m_lastIndex( 0 )
{
    ui->setupUi( this );

    connect( ui->scrobbles, SIGNAL( trackClicked(TrackWidget&)), SLOT( onTrackClicked(TrackWidget&)));
    connect( ui->layout, SIGNAL( moveFinished(QLayoutItem*)), SLOT(onMoveFinished(QLayoutItem*)));

    ui->stackedWidget->setCurrentWidget( ui->scrobbles );
}

ScrobblesWidget::~ScrobblesWidget()
{
    delete ui;
}

void
ScrobblesWidget::refresh()
{
    ui->scrobbles->refresh();
}

void
ScrobblesWidget::onCurrentChanged( int index )
{
    if ( index == 1 && (m_lastIndex != index || ui->layout->currentWidget() != ui->stackedWidget ) )
    {
        // We've switch to this tab OR are moving back from Scrobble to ScrobbleList
        ui->scrobbles->refresh();
        AnalyticsService::instance().sendPageView( "Scrobbles" );
    }

    m_lastIndex = index;

    ui->layout->moveToWidget( ui->stackedWidget );
}

void
ScrobblesWidget::onTrackClicked( TrackWidget& trackWidget )
{
    MetadataWidget* w;
    ui->layout->addWidget( w = new MetadataWidget( trackWidget.track() ));
    w->fetchTrackInfo();
    w->setBackButtonVisible( true );

    trackWidget.startSpinner();
    connect( ui->layout, SIGNAL( moveFinished(QLayoutItem*)), &trackWidget, SLOT(clearSpinner()) );

    connect( w, SIGNAL(finished()), SLOT(onMetadataWidgetFinished()));
    connect( w, SIGNAL(backClicked()), SLOT(onBackClicked()));
}

void
ScrobblesWidget::onMetadataWidgetFinished()
{
    ui->layout->moveForward();
    AnalyticsService::instance().sendPageView( "Scrobbles/Scrobble" );
}

void
ScrobblesWidget::onBackClicked()
{
    ui->layout->moveToWidget( ui->stackedWidget );
    AnalyticsService::instance().sendPageView( "Scrobbles" );
}

void
ScrobblesWidget::onMoveFinished( QLayoutItem* i )
{
    if( i->widget() == ui->stackedWidget )
    {
        while ( ui->layout->count() > 1 )
        {
            QLayoutItem* item = ui->layout->takeAt( 1 );
            item->widget()->deleteLater();
            delete item;
        }
    }
}
