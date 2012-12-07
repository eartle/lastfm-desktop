#ifndef FRIENDWIDGET_H
#define FRIENDWIDGET_H

#include <QFrame>
#include <QPointer>

#include <lastfm/XmlQuery.h>
#include <lastfm/User.h>
#include <lastfm/Track.h>

namespace Ui { class FriendWidget; }
namespace unicorn { class Label; }
using unicorn::Label;

class FriendWidget : public QFrame
{
    Q_OBJECT
public:
    explicit FriendWidget( const lastfm::XmlQuery& user, QWidget *parent = 0 );

    void update( const lastfm::XmlQuery& user, unsigned int order );
    void setOrder( int order );

    QString name() const;
    QString realname() const;

    void setListeningNow( bool listeningNow );

    bool operator<( const FriendWidget& that ) const;

    static QString genderString( const lastfm::Gender& gender );
    static QString userString( const lastfm::User& user );

private:
    void setDetails();

private slots:
    void updateTimestamp();

private:
    Ui::FriendWidget* ui;

    lastfm::User m_user;
    lastfm::MutableTrack m_track;
    unsigned int m_order;
    bool m_listeningNow;

    QPointer<QMovie> m_movie;
    QPointer<QTimer> m_timestampTimer;
};

#endif // FRIENDWIDGET_H
