
#pragma once

#include "../UnicornSession.h"
#include "PlayBus.h"

namespace unicorn
{

class Bus : public PlayBus
{
    Q_OBJECT
public:
    Bus( QObject* parent = 0 );

    bool isWizardRunning();
    QMap<QString, QString> getSessionData();
    void announceSessionChange( unicorn::Session& s );

private slots:
    void onMessage( const QByteArray& message );
    void onQuery( const QString& uuid, const QByteArray& message );

signals:
    void wizardRunningQuery( const QString& uuid );
    void sessionQuery( const QString& uuid );
    void sessionChanged( const unicorn::Session& s );
    void rosterUpdated();
    void lovedStateChanged(bool loved);
};

}
