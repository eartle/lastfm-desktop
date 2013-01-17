#ifndef USER_MANAGER_WIDGET_H_
#define USER_MANAGER_WIDGET_H_

#include <QLayout>
#include <QRadioButton>
#include <QPointer>

#include "lib/DllExportMacro.h"

namespace lastfm{ class User; }
namespace unicorn
{
    class LoginProcess;
    class Session;
}

class LoginContinueDialog;

class QAbstractButton;
class QButtonGroup;
class QFrame;
class QLabel;
class QPushButton;
class QVBoxLayout;

class UNICORN_DLLEXPORT UserRadioButton : public QHBoxLayout
{
Q_OBJECT

    friend class UserManagerWidget;

public:
    UserRadioButton( const lastfm::User& user );
    const QString user() const;

    void click();
    bool isChecked() const;

signals:
    void clicked();
    void remove();

private:
    void setUser( const lastfm::User& user );

private slots:
    void onUserFetched();
    void onSessionChanged( const unicorn::Session& session );

private:
    struct
    {
        QRadioButton* button;
        QPushButton* remove;
        QLabel* username;
        QLabel* realName;
        QLabel* loggedIn;
        class AvatarWidget* image;
        QFrame* frame;
    } ui;
};


class UNICORN_DLLEXPORT UserManagerWidget : public QWidget
{
Q_OBJECT
public:
    UserManagerWidget( QWidget* parent = 0 );
    ~UserManagerWidget();

    UserRadioButton* checkedButton() const;

signals:
    void rosterUpdated();
    void userChanged();

protected slots:
    void onAddUserClicked();
    void onLoginDialogAccepted();
    void onLoginContinueDialogAccepted();
    void onUserRemoved();

protected:
    void add( UserRadioButton*, bool = true );

    struct {
        class QGroupBox* groupBox;
        class QVBoxLayout* usersLayout;
        class QPushButton* addUserButton;
    } ui;


private slots:
    void onLoginComplete( const unicorn::Session& session );

private:
    QPointer<unicorn::LoginProcess> m_loginProcess;
    QPointer<LoginContinueDialog> m_lcd;
};

#endif //USER_MANAGER_WIDGET_H_
