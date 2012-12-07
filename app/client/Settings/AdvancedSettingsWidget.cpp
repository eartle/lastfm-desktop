/*
   Copyright 2010 Last.fm Ltd.
      - Primarily authored by Jono Cole, Michael Coffey, and William Viana

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

#include <QApplication>
#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QStringListModel>
#include <QNetworkProxy>

#include <lastfm/User.h>
#include <lastfm/ws.h>
#include <lastfm/NetworkAccessManager.h>

#include "lib/unicorn/QMessageBoxBuilder.h"
#include "lib/unicorn/UnicornApplication.h"
#include "lib/unicorn/UnicornSession.h"
#include "lib/unicorn/widgets/UserManagerWidget.h"

#include "../Widgets/ShortcutEdit.h"
#include "../AudioscrobblerSettings.h"
#include "../Application.h"
#include "ui_AdvancedSettingsWidget.h"
#include "AdvancedSettingsWidget.h"

using lastfm::User;

AdvancedSettingsWidget::AdvancedSettingsWidget( QWidget* parent )
    : SettingsWidget( parent ),
      ui( new Ui::AdvancedSettingsWidget )
{
    ui->setupUi( this );

#ifdef Q_WS_X11
    ui->shortcuts->hide();
#else
    AudioscrobblerSettings settings;
    ui->sce->setTextValue( settings.raiseShortcutDescription() );
    ui->sce->setModifiers( settings.raiseShortcutModifiers() );
    ui->sce->setKey( settings.raiseShortcutKey() );

    connect( ui->sce, SIGNAL(editTextChanged(QString)), this, SLOT(onSettingsChanged()));
#endif
    unicorn::AppSettings appSettings;

    QStringList proxyTypes;
    proxyTypes << tr( "Auto-detect" ) << tr( "No-proxy" ) << tr("HTTP") << tr("SOCKS5");
    ui->proxyType->addItems( proxyTypes );
    ui->proxyType->setCurrentIndex( appSettings.value( "proxyType", 0 ).toInt() );
    ui->proxyHost->setText( appSettings.value( "proxyHost", "" ).toString() );
    ui->proxyPort->setText( appSettings.value( "proxyPort", "" ).toString() );
    ui->proxyUsername->setText( appSettings.value( "proxyUsername", "" ).toString() );
    ui->proxyPassword->setText( appSettings.value( "proxyPassword", "" ).toString() );

    connect( ui->proxyType, SIGNAL(currentIndexChanged(int)), this, SLOT(onSettingsChanged()));
    connect( ui->proxyHost, SIGNAL(textChanged(QString)), this, SLOT(onSettingsChanged()));
    connect( ui->proxyPort, SIGNAL(textChanged(QString)), this, SLOT(onSettingsChanged()));
    connect( ui->proxyUsername, SIGNAL(textChanged(QString)), this, SLOT(onSettingsChanged()));
    connect( ui->proxyPassword, SIGNAL(textChanged(QString)), this, SLOT(onSettingsChanged()));
}

void
AdvancedSettingsWidget::saveSettings()
{
    qDebug() << "has unsaved changes?" << hasUnsavedChanges();

    if ( hasUnsavedChanges() )
    {
        AudioscrobblerSettings settings;
        settings.setRaiseShortcutKey( ui->sce->key() );
        settings.setRaiseShortcutModifiers( ui->sce->modifiers() );
        settings.setRaiseShortcutDescription( ui->sce->textValue() );

        aApp->setRaiseHotKey( ui->sce->modifiers(), ui->sce->key() );

        unicorn::AppSettings appSettings;
        int proxyType = appSettings.value( "proxyType", 0 ).toInt();
        QString proxyHost = appSettings.value( "proxyHost", "" ).toString();
        QString proxyPort = appSettings.value( "proxyPort", "" ).toString();
        QString proxyUsername = appSettings.value( "proxyUsername", "" ).toString();
        QString proxyPassword = appSettings.value( "proxyPassword", "" ).toString();

        if ( proxyType != ui->proxyType->currentIndex()
             || proxyHost != ui->proxyHost->text()
             || proxyPort != ui->proxyPort->text()
             || proxyUsername != ui->proxyUsername->text()
             || proxyPassword != ui->proxyPassword->text() )
        {
            // one of the proxy settings has changed

            // save them
            appSettings.setValue( "proxyType", ui->proxyType->currentIndex() );
            appSettings.setValue( "proxyHost", ui->proxyHost->text() );
            appSettings.setValue( "proxyPort", ui->proxyPort->text() );
            appSettings.setValue( "proxyUsername", ui->proxyUsername->text() );
            appSettings.setValue( "proxyPassword", ui->proxyPassword->text() );

            // set this new proxy
            QNetworkProxy::ProxyType type = QNetworkProxy::DefaultProxy;

            if ( ui->proxyType->currentIndex() == 1 )
                type = QNetworkProxy::NoProxy;
            else if ( ui->proxyType->currentIndex() == 2 )
                type = QNetworkProxy::HttpProxy;
            else if ( ui->proxyType->currentIndex() == 3 )
                type = QNetworkProxy::Socks5Proxy;

            QNetworkProxy proxy( type, ui->proxyHost->text(), ui->proxyPort->text().toInt(), ui->proxyUsername->text(), ui->proxyPassword->text() );
            lastfm::NetworkAccessManager* nam = qobject_cast<lastfm::NetworkAccessManager*>( lastfm::nam() );

            if ( nam )
                nam->setUserProxy( proxy );
        }
    }
}
