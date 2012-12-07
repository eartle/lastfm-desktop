#ifndef SCROBBLES_LIST_WIDGET_H
#define SCROBBLES_LIST_WIDGET_H

#include <QListWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QPointer>

#include <lastfm/Track.h>

namespace lastfm { class Track; }
using lastfm::Track;

namespace unicorn { class Session; }

class QNetworkReply;

class ScrobblesListWidget : public QListWidget
{
    Q_OBJECT
public:
    ScrobblesListWidget( QWidget* parent = 0 );

signals:
    void trackClicked( class TrackWidget& );

public slots:
    void refresh();

private slots: 
    void onItemClicked( class TrackWidget& index );
    void onMoreClicked();

    void onTrackStarted( const Track& track, const Track& );

    void onSessionChanged( const unicorn::Session& session );

    void onResumed();
    void onPaused();
    void onStopped();

    void onGotRecentTracks();

    void onScrobblesSubmitted( const QList<lastfm::Track>& tracks );

    void onTrackWidgetRemoved();

    void write();
    void doWrite();

#ifdef Q_OS_MAC
    void scroll();
#endif

private:
    QString price( const QString& price, const QString& currency ) const;

    void read();

    QList<lastfm::Track> addTracks( const QList<lastfm::Track>& tracks );
    void limit( int limit );

    void hideScrobbledNowPlaying();

    void showEvent(QShowEvent *);

    void fetchTrackInfo( const QList<lastfm::Track>& tracks );

    void mousePressEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );

    void onRefreshing( bool refreshing );

private:
    QString m_path;

    QPointer<QTimer> m_writeTimer;
    QPointer<QNetworkReply> m_recentTrackReply;

    lastfm::Track m_track;
    class ScrobblesListWidgetItem* m_refreshItem;
    class ScrobblesListWidgetItem* m_trackItem;
    class ScrobblesListWidgetItem* m_moreItem;
};


#endif //ACTIVITY_LIST_WIDGET_H

