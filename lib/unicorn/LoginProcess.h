#ifndef LOGIN_PROCESS_H
#define LOGIN_PROCESS_H

#include <QObject>
#include <QPointer>

#include <lastfm/ws.h>

#include "UnicornSession.h"

#include "lib/DllExportMacro.h"

namespace unicorn
{
/**
 * This class encapsulates the whole login process.
 *
 * Call the authenticate function to start the login process
 * and connect to the gotSession signal to be notified when
 * the process finishes.
 */
class UNICORN_DLLEXPORT LoginProcess : public QObject
{
    Q_OBJECT
public:
    LoginProcess( QObject* parent = 0 );
    ~LoginProcess();

private:
    void handleError( const lastfm::XmlQuery& lfm );

signals:
    void authUrlChanged( const QString& authUrl );

public slots:
    void authenticate();
    void getSession();

private slots:
    void onGotToken();
    void onGotSession();

private: 
    QString m_token;
};

}// namespace unicorn

#endif // LOGIN_PROCESS_H
