
#include "Updater.h"

#ifndef Q_OS_MAC

#ifdef Q_OS_WIN
#include <QCoreApplication>
#include <QStringList>
#include <winsparkle.h>
#endif

unicorn::Updater::Updater(QObject *parent) :
    QObject(parent)
{
#ifdef Q_OS_WIN
    win_sparkle_init();

    if ( qApp->arguments().contains( "--update" ) )
        win_sparkle_set_appcast_url( "http://users.last.fm/~michael/updates_win.xml" );
    else if ( qApp->arguments().contains( "--update-static" ) )
        win_sparkle_set_appcast_url( "http://static.last.fm/client/Mac/updates.xml" );
    else
        win_sparkle_set_appcast_url( "http://cdn.last.fm/client/Win/updates.xml" );
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
