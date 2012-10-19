#ifndef SKIPLISTENER_H
#define SKIPLISTENER_H

#include <QObject>
#include <QStringList>

namespace lastfm { class RadioStation; }

class SkipListener : public QObject
{
    Q_OBJECT
public:
    explicit SkipListener(QObject *parent = 0);

signals:

private slots:
    void onTrackSpooled();
    void onNewConnection();

private:
    QStringList users( const lastfm::RadioStation& rs );

private:
    class QTcpServer* m_server;
    QStringList m_skippers;
};

#endif // SKIPLISTENER_H
