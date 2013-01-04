#ifndef UNICORN_SESSION_H_
#define UNICORN_SESSION_H_

#include <lastfm/XmlQuery.h>
#include <lastfm/misc.h>
#include <lastfm/User.h>
#include <lastfm/ws.h>

#include <QObject>
#include <QSharedData>

#include "lib/DllExportMacro.h"

namespace unicorn {

class UNICORN_DLLEXPORT Session : public QObject
{
    Q_OBJECT
public:
    /** Return session object from stored session */
    Session();
    Session( QDataStream& dataStream );
    Session( const QString& username, QString sessionKey = "" );

    bool isValid() const;

    // client radio permissions
    bool youRadio() const;
    bool registeredRadio() const;
    bool subscriberRadio() const;

    // web radio permissions
    bool youWebRadio() const;
    bool registeredWebRadio() const;
    bool subscriberWebRadio() const;

    QString subscriptionPriceString() const;

    QString sessionKey() const;
    lastfm::User user() const;

    static QNetworkReply* getToken();
    static QNetworkReply* getSession( QString token );
    static QMap<QString, QString> lastSessionData();

    QDataStream& write( QDataStream& out ) const;
    QDataStream& read( QDataStream& in );

signals:
    void userInfoUpdated( const lastfm::User& user );
    void sessionChanged( const unicorn::Session& session );

protected:
    void init( const QString& username, const QString& sessionKey );

private:
    void cacheUserInfo( const lastfm::User& user );
    void cacheSessionInfo( const unicorn::Session& session );

private slots:
    void fetchInfo();
    void onUserGotInfo();
    void onAuthGotSessionInfo();

private:
    QString m_prevUsername;
    QString m_sessionKey;
    lastfm::User m_user;

    QString m_subscriptionPrice;

    bool m_valid;

    bool m_youRadio;
    bool m_registeredRadio;
    bool m_subscriberRadio;

    bool m_youWebRadio;
    bool m_registeredWebRadio;
    bool m_subscriberWebRadio;
};

}

QDataStream& operator<<( QDataStream& out, const unicorn::Session& s );
QDataStream& operator>>( QDataStream& in, unicorn::Session& s );

#endif //UNICORN_SESSION_H_
