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

private slots:
    void onSessionChanged( const unicorn::Session& session );

    void oniTunesClicked();

private:
    Ui::NothingPlayingWidget* ui;
};

#endif // NOTHINGPLAYINGWIDGET_H
