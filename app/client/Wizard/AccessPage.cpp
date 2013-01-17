/*
   Copyright 2005-2009 Last.fm Ltd. 
      - Primarily authored by Jono Cole

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

#include <QVBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QAbstractButton>
#include <QTextEdit>

#include "lib/unicorn/LoginProcess.h"
#include "lib/unicorn/UnicornSession.h"

#include "../Application.h"

#include "FirstRunWizard.h"

#include "AccessPage.h"

AccessPage::AccessPage()
    :m_valid( false ),
      m_gotUserInfo( false )
{
    QHBoxLayout* layout = new QHBoxLayout( this );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 20 );
    
    layout->addWidget( ui.image = new QLabel(), 1, Qt::AlignTop | Qt::AlignHCenter );
    ui.image->setObjectName( "image" );

    QVBoxLayout* rL = new QVBoxLayout( this );
    rL->setContentsMargins( 0, 0, 0, 0 );
    rL->setSpacing( 20 );

    layout->addLayout( rL, 1 );

    rL->addWidget( ui.description = new QLabel( "" ), 1, Qt::AlignTop);
    ui.description->setObjectName( "description" );
    ui.description->setWordWrap( true );

    rL->addWidget( ui.authUrl = new QTextEdit( "" ) );
    ui.authUrl->setTextInteractionFlags( Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard );
    ui.authUrl->setObjectName( "authUrl" );
}

void
AccessPage::initializePage()
{
    setTitle( tr( "We're waiting for you to connect to Last.fm" ));

    ui.description->setText( tr( "<p>Please click the <strong>Yes, Allow Access</strong> button in your web browser to connect your Last.fm account to the Last.fm Desktop App.</p>"
                                 "<p>If you haven't connected because you closed the browser window or you clicked cancel, please try again.</p>" ) );

    wizard()->setCommitPage( true );

    if ( wizard()->canGoBack() )
        wizard()->setButton( FirstRunWizard::BackButton, tr( "<< Back" ) );
    wizard()->setButton( FirstRunWizard::NextButton, tr( "Continue" ) );
    QAbstractButton* custom = wizard()->setButton( FirstRunWizard::CustomButton, tr( "Try Again" ) );

    connect( custom, SIGNAL(clicked()), SLOT(tryAgain()));

    disconnect( aApp, SIGNAL(sessionChanged(unicorn::Session)), this, SLOT(onSessionChanged(unicorn::Session)));
    disconnect( aApp, SIGNAL(gotUserInfo(lastfm::User)), this, SLOT(onGotUserInfo(lastfm::User)));
    connect( aApp, SIGNAL(sessionChanged(unicorn::Session)), SLOT(onSessionChanged(unicorn::Session)));
    connect( aApp, SIGNAL(gotUserInfo(lastfm::User)), SLOT(onGotUserInfo(lastfm::User)));

    tryAgain();
}

void
AccessPage::tryAgain()
{
    if ( !m_loginProcess )
    {
        m_loginProcess = new unicorn::LoginProcess( this );
        connect( m_loginProcess, SIGNAL(authUrlChanged(QString)), SLOT(onAuthUrlChanged(QString)));
    }

    m_loginProcess->authenticate();

    // disable the try again and coninue buttons
    wizard()->setButton( FirstRunWizard::NextButton, tr( "Continue" ) )->setEnabled( false );
    wizard()->setButton( FirstRunWizard::CustomButton, tr( "Try Again" ) )->setEnabled( false );
}

void
AccessPage::onAuthUrlChanged( const QString& authUrl )
{
    ui.description->setText( tr( "<p>Please click the <strong>Yes, Allow Access</strong> button in your web browser to connect your Last.fm account to the Last.fm Desktop App.</p>"
                                                       "<p>If you haven't connected because you closed the browser window or you clicked cancel, please try again.</p>" )
                                                   + tr( "<p>If your web browser didn't open, copy and paste the link below into your address bar.</p>" ) );

    ui.authUrl->setText( authUrl );

    wizard()->setButton( FirstRunWizard::CustomButton, tr( "Try Again" ) )->setEnabled( true );

    if ( !authUrl.isEmpty() )
        wizard()->setButton( FirstRunWizard::NextButton, tr( "Continue" ) )->setEnabled( true );
}

void
AccessPage::onGotUserInfo( const lastfm::User& /*user*/ )
{
    m_gotUserInfo = true;

    checkComplete();
}

void
AccessPage::onSessionChanged( const unicorn::Session& session )
{
    if ( session.isValid() )
        checkComplete();
}

void
AccessPage::checkComplete()
{
    if ( aApp->currentSession().isValid() && m_gotUserInfo && !m_valid )
    {
        disconnect( aApp, SIGNAL(sessionChanged(unicorn::Session)), this, SLOT(onSessionChanged(unicorn::Session)));
        disconnect( aApp, SIGNAL(gotUserInfo(lastfm::User)), this, SLOT(onGotUserInfo(lastfm::User)));

        // we've now got both the session info and the user info
        m_valid = true;

        wizard()->showWelcome();
        wizard()->next();

        m_loginProcess->deleteLater();
    }
}

void
AccessPage::cleanupPage()
{
    ui.description->clear();
    ui.authUrl->clear();
}


bool
AccessPage::validatePage()
{
    if ( m_valid )
        return true;

    // try to get the session key!
    m_loginProcess->getSession();

    return false;
}

