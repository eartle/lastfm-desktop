#ifndef NAMEDPIPESERVER_H
#define NAMEDPIPESERVER_H

#include <QThread>

class NamedPipeServer : public QThread
{
    Q_OBJECT
public:
    explicit NamedPipeServer( QObject* parent = 0 );

private:
    void run();

signals:
    QString lineReady( const QString& line );
};

#endif // NAMEDPIPESERVER_H
