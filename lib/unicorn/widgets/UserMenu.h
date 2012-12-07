#ifndef USER_MENU_H_
#define USER_MENU_H_

#include <QMenu>
#include <QPointer>

#include "lib/DllExportMacro.h"
#include "lib/unicorn/UnicornSession.h"

class UNICORN_DLLEXPORT UserMenu : public QMenu
{
    Q_OBJECT
public:
    UserMenu( QWidget* p = 0 );

protected slots:
    void onSessionChanged( const unicorn::Session& session );

    void onTriggered( QAction* a );
    void manageUsers();
    void refresh();
    void subscribe();

private:
    QPointer<QAction> m_subscribe;
};

#endif
