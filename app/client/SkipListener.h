#pragma once

#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QMap>

namespace lastfm { class RadioStation; }

class SkipListener : public QObject
{
    Q_OBJECT
public:
    explicit SkipListener(QObject *parent = 0);

private slots:
    void onTrackSpooled();
    void onNewConnection();

private:
    QStringList users( const lastfm::RadioStation& rs );
    void sendMessage( const QString& message );

private:
    class QTcpServer* m_server;
    QStringList m_skippers;
    QMap<QString, QDateTime> m_cueRequests;
};
