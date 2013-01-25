#ifndef NOTHINGPLAYINGWIDGET_H
#define NOTHINGPLAYINGWIDGET_H

#include <QFrame>

namespace lastfm { class User; }
namespace unicorn { class Session; }

namespace Ui { class NothingPlayingWidget; }

class NothingPlayingWidget : public QFrame
{
    Q_OBJECT
public:
    explicit NothingPlayingWidget( QWidget* parent = 0 );

private:
    void setUser( const lastfm::User& user );
#ifdef Q_OS_WIN
    void startApp( const QString& app );
#endif

private slots:
    void onSessionChanged( const unicorn::Session& session );

#if defined( Q_OS_MAC ) || defined( Q_OS_WIN )
    void oniTunesClicked();
#endif
#ifdef Q_OS_WIN
    void onWinampClicked();
    void onWMPClicked();
    void onFoobarClicked();
#endif

private:
    Ui::NothingPlayingWidget* ui;
};

#endif // NOTHINGPLAYINGWIDGET_H
