#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <QPointer>
#include <QFrame>

#include <lastfm/Track.h>

#include "lib/unicorn/UnicornSession.h"

namespace unicorn { class Label; }

namespace Ui { class ProfileWidget; }

class ProfileWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ProfileWidget(QWidget *parent = 0);
    ~ProfileWidget();

public slots:
    void onCurrentChanged( int index );
    void refresh();

private slots:
    void onSessionChanged( const unicorn::Session& session );
    void onGotUserInfo( const lastfm::User& userDetails );

    void onGotTopWeeklyArtists();
    void onGotTopOverallArtists();

    void onGotLibraryArtists();

    void onGotLovedTracks();

    void onScrobblesCached( const QList<lastfm::Track>& tracks );
    void onScrobbleStatusChanged( short scrobbleStatus );
    void setScrobbleCount();

private:
    Ui::ProfileWidget* ui;

    QString m_currentUser;
    int m_scrobbleCount;
};

#endif // PROFILEWIDGET_H
