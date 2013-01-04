/*
   Copyright 2005-2009 Last.fm Ltd. 
      - Primarily authored by Jono Cole

   This file is part of the Last.fm Desktop Application Suite.

   lastfm-desktop is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   lastfm-desktop is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with lastfm-desktop.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef ACCESS_PAGE_H
#define ACCESS_PAGE_H

#include "WizardPage.h"
#include <QPointer>

namespace lastfm { class User; }

namespace unicorn{ 
    class LoginProcess;
    class Session;
}

class AccessPage : public WizardPage
{
    Q_OBJECT
private:
    struct
    {
        class QLabel* image;
        class QLabel* description;
    } ui;

public:
    AccessPage();

    void initializePage();
    bool validatePage();
    void cleanupPage();

private:
    void checkComplete();

private slots:
    void tryAgain();

    void onGotUserInfo( const lastfm::User& user );
    void onSessionChanged( const unicorn::Session& session );

protected:
    QList<unicorn::LoginProcess*> m_loginProcesses;
    bool m_valid;
    bool m_gotUserInfo;
};

#endif //AUTH_IN_PROGRESS_PAGE_H

