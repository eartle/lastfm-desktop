
#include <QHBoxLayout>
#include <QLabel>
#include <QDesktopServices>
#include <QAbstractButton>

#include <lastfm/UrlBuilder.h>

#include "lib/unicorn/DesktopServices.h"

#include "../Application.h"

#include "FirstRunWizard.h"
#include "TourRadioPage.h"

TourRadioPage::TourRadioPage()
{
    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 20 );

    layout->addWidget( ui.image = new QLabel( this ), 0, Qt::AlignTop | Qt::AlignHCenter );
    ui.image->setObjectName( "image" );
    layout->addWidget( ui.description = new QLabel( "", this ), 0, Qt::AlignTop);
    ui.description->setObjectName( "description" );
    ui.description->setWordWrap( true );
}


void
TourRadioPage::initializePage()
{
    if ( wizard()->user().isSubscriber() )
    {
        setTitle( tr( "Listen to non-stop, personalised radio" ) );
        ui.description->setText( tr( "<p>Use the Last.fm Desktop App to listen to personalised radio based on the music you want to hear.</p>"
                                 "<p>Every play of every Last.fm station is totally different, from stations based on artists and tags to brand new recommendations tailored to your music taste.</p>" ) );
    }
    else
    {
        setTitle( tr( "Subscribe and listen to non-stop, personalised radio*" ) );
        ui.description->setText( tr( "<p>Subscribe to Last.fm and use the Last.fm Desktop App to listen to personalised radio based on the music you want to hear.</p>"
                                 "<p>Every play of every Last.fm station is totally different, from stations based on artists and tags to brand new recommendations tailored to your music taste.</p>"
                                 "<p>* Radio is a subscriber-only feature outside of the UK, US and Germany</p>" ) );

        QAbstractButton* custom = wizard()->setButton( FirstRunWizard::CustomButton, tr( "Subscribe" ) );
        connect( custom, SIGNAL(clicked()), SLOT(subscribe()));
    }

    wizard()->setButton( FirstRunWizard::NextButton, tr( "Continue" ) );
    if ( wizard()->canGoBack() )
        wizard()->setButton( FirstRunWizard::BackButton, tr( "<< Back" ) );
    wizard()->setButton( FirstRunWizard::SkipButton, tr( "Skip Tour >>" ) );
}

void
TourRadioPage::cleanupPage()
{
}

void
TourRadioPage::subscribe()
{
    unicorn::DesktopServices::openUrl( lastfm::UrlBuilder( "subscribe" ).url() );
}
