
#include "Updater.h"

#ifndef Q_OS_MAC

#ifdef Q_OS_WIN
#include <QCoreApplication>
#include <QStringList>
#include <winsparkle.h>
#endif

#include "lib/unicorn/UnicornSettings.h"
#include "lib/unicorn/UnicornApplication.h"

unicorn::Updater::Updater(QObject *parent) :
    QObject(parent)
{
#ifdef Q_OS_WIN
    win_sparkle_init();

    if ( qApp->arguments().contains( "--update" ) )
    {
        int urlIndex = qApp->arguments().indexOf( "--update" ) + 1;

        if ( qApp->arguments().count() > urlIndex && qApp->arguments()[urlIndex].startsWith( "http://" ) )
            win_sparkle_set_appcast_url( qApp->arguments()[urlIndex].toUtf8() );
        else
            setBetaUpdates( unicorn::Settings().value( SETTING_BETA_UPGRADES, false ).toBool() );
    }
    else
        setBetaUpdates( unicorn::Settings().value( SETTING_BETA_UPGRADES, false ).toBool() );
#endif
}

void
unicorn::Updater::setBetaUpdates( bool betaUpdates )
{
#ifdef Q_OS_WIN
    win_sparkle_set_appcast_url( betaUpdates ? UPDATE_URL_WIN_BETA : UPDATE_URL_WIN );
#endif
}

void
unicorn::Updater::checkForUpdates()
{
#ifdef Q_OS_WIN
    win_sparkle_check_update_with_ui();
#endif
}

unicorn::Updater::~Updater()
{
#ifdef Q_OS_WIN
    win_sparkle_cleanup();
#endif
}

#endif // Q_OS_MAC
