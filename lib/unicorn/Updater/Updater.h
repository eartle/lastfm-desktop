#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

#include "lib/DllExportMacro.h"

#ifdef Q_OS_WIN
#define UPDATE_URL_WIN "http://cdn.last.fm/client/updates/updates.win.xml"
#define UPDATE_URL_WIN_BETA "http://cdn.last.fm/client/updates/updates.win.beta.xml"
#elif defined( Q_OS_MAC )
#define UPDATE_URL_MAC @"http://cdn.last.fm/client/updates/updates.mac.xml"
#define UPDATE_URL_MAC_BETA @"http://cdn.last.fm/client/updates/updates.mac.beta.xml"
#endif

namespace unicorn
{

class UNICORN_DLLEXPORT Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);
    ~Updater();

    void setBetaUpdates( bool );

public slots:
    void checkForUpdates();
};

}

#endif // UPDATER_H
