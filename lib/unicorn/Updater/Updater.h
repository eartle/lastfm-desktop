#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>

#include "lib/DllExportMacro.h"

namespace unicorn
{

class UNICORN_DLLEXPORT Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);
    ~Updater();

public slots:
    void checkForUpdates();
};

}

#endif // UPDATER_H
