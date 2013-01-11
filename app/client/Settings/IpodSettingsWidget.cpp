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

#include "ui_IpodSettingsWidget.h"
#include "IpodSettingsWidget.h"

#ifdef Q_WS_X11
#include "../MediaDevices/IpodDevice_linux.h"
#endif

#ifdef Q_OS_WIN
#include "../Plugins/ITunesPluginInfo.h"
#endif

#include "lib/unicorn/UnicornApplication.h"
#include "../Dialogs/CloseAppsDialog.h"
#include "lib/unicorn/QMessageBoxBuilder.h"

#include "../MediaDevices/IpodDevice.h"

#include "lib/unicorn/UnicornSettings.h"
#include "lib/unicorn/Widgets/Label.h"

#include <lastfm/User.h>

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QTreeWidget>
#include <QVBoxLayout>

IpodSettingsWidget::IpodSettingsWidget( QWidget* parent )
    : SettingsWidget( parent ),
      ui( new Ui::IpodSettingsWidget )
{
    ui->setupUi( this );

    ui->alwaysAsk->setChecked( unicorn::AppSettings().value( SETTING_ALWAYS_ASK, true ).toBool() );
    connect( ui->alwaysAsk, SIGNAL(clicked(bool)), SLOT(onSettingsChanged()));

#ifdef Q_WS_X11
    ui->deviceScrobblingEnabled->hide();
#else
    ui->deviceScrobblingEnabled->setChecked( unicorn::AppSettings( OLDE_PLUGIN_SETTINGS ).value( SETTING_OLDE_ITUNES_DEVICE_SCROBBLING_ENABLED, true ).toBool() );
    connect( ui->deviceScrobblingEnabled, SIGNAL(clicked(bool)), SLOT(onSettingsChanged()));
#endif

    ui->note->setText( unicorn::Label::boldLinkStyle( tr( "<p>Please note:</p>"
                           "<p>Using an iOS scrobbling app, like %1, may result in double scrobbles. Please only enable scrobbling in one of them.</p>"
                           "<p>iTunes Match synchronises play counts, but not last played times, across multiple devices. This will lead to duplicate scrobbles, at incorrect times. For now, we recommend iTunes Match users disable device scrobbling on desktop devices and scrobble iPhones/iPods using an iOS scrobbling app, like %1.</p>" ).arg( unicorn::Label::anchor( "itmss://itunes.apple.com/gb/app/scrobbler-for-ios/id585235199", "Scrobbler for iOS" ) ), Qt::black ) );
}

void
IpodSettingsWidget::saveSettings()
{
    if ( hasUnsavedChanges() )
    {
        // save settings
        qDebug() << "Saving settings...";

        unicorn::AppSettings().setValue( SETTING_ALWAYS_ASK, ui->alwaysAsk->isChecked() );

        // we need to restart iTunes for this setting to take affect
        bool currentlyEnabled = unicorn::AppSettings( OLDE_PLUGIN_SETTINGS ).value( SETTING_OLDE_ITUNES_DEVICE_SCROBBLING_ENABLED, true ).toBool();

#ifndef Q_WS_X11
        if ( currentlyEnabled != ui->deviceScrobblingEnabled->isChecked() )
        {
#ifdef Q_OS_WIN
            QList<IPluginInfo*> plugins;
            ITunesPluginInfo* iTunesPluginInfo = new ITunesPluginInfo;
            plugins << iTunesPluginInfo;
            CloseAppsDialog* closeApps = new CloseAppsDialog( plugins, this );
            closeApps->setOwnsPlugins( true );
#else
            CloseAppsDialog* closeApps = new CloseAppsDialog( this );
#endif
            if ( closeApps->result() != QDialog::Accepted )
                closeApps->exec();
            else
                closeApps->deleteLater();

            if ( closeApps->result() == QDialog::Accepted )
            {
                unicorn::AppSettings( OLDE_PLUGIN_SETTINGS ).setValue( SETTING_OLDE_ITUNES_DEVICE_SCROBBLING_ENABLED, ui->deviceScrobblingEnabled->isChecked() );
            }
            else
            {
                ui->deviceScrobblingEnabled->setChecked( currentlyEnabled );

                // The user didn't close their media players
                QMessageBoxBuilder( this ).setTitle( tr( "Setting not changed" ) )
                        .setIcon( QMessageBox::Warning )
                        .setText( tr( "You did not close iTunes for this setting to change" ) )
                        .setButtons( QMessageBox::Ok )
                        .exec();
            }
        }
#endif

        onSettingsSaved();
    }
}
