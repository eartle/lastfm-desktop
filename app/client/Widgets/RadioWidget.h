#ifndef RADIOWIDGET_H
#define RADIOWIDGET_H

#include <QPointer>
#include <QFrame>

namespace lastfm { class User; }
namespace lastfm { class RadioStation; }
namespace lastfm { class Track; }
using lastfm::RadioStation;
using lastfm::Track;

namespace unicorn { class Session; }

namespace Ui { class RadioWidget; }

class RadioWidget : public QFrame
{
    Q_OBJECT
public:
    explicit RadioWidget(QWidget *parent = 0);
    ~RadioWidget();

private slots:
    void onSessionChanged( const unicorn::Session& session );

    void onTuningIn( const RadioStation& station );
    void onRadioStopped();
    void onTrackStarted( const Track& track , const Track& oldTrack );

    void onGotRecentStations();

    void onSubscribeClicked();
    void onListenClicked();

private:
    void refresh( const unicorn::Session& session );

private:
    Ui::RadioWidget* ui;

    QString m_currentUsername;

    QPointer<QMovie> m_movie;
};

#endif // RADIOWIDGET_H
