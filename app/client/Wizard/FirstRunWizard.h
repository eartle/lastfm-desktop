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
#ifndef FIRST_RUN_WIZARD_H_
#define FIRST_RUN_WIZARD_H_

#include <QDialog>
#include <QAbstractButton>
#include <QList>
#include <QPointer>

#include "lib/unicorn/UnicornSession.h"

#ifdef Q_OS_WIN32
#include "../Plugins/PluginList.h"
#endif

namespace Ui { class FirstRunWizard; }

class FirstRunWizard : public QDialog
{
    Q_OBJECT
public:
    enum Button
    {
        CustomButton,
        BackButton,
        SkipButton,
        NextButton,
        FinishButton
    };

public:
    FirstRunWizard( bool startFromTour = false, QWidget* parent = 0 );
    ~FirstRunWizard();

    void setTitle( const QString& title );

    QAbstractButton* setButton( Button button, const QString& text );

    void setCommitPage( bool commitPage );
    bool canGoBack() const;

    void showWelcome();

#ifdef Q_OS_WIN32
    class PluginList* pluginList() const { return m_plugins; }
#endif

public slots:
    void next();
    void back();
    void skip();

private slots:
    void onWizardCompleted();
    void onRejected();

    void onBootstrapStarted( const QString& pluginId );
    void onBootstrapDone( int status );

private:
    void initializePage( QWidget* page );
    void cleanupPage( QWidget* page );

private:
    Ui::FirstRunWizard* ui;
#ifdef Q_OS_WIN32
    QPointer<PluginList> m_plugins;
#endif

    QList<QWidget*> m_pages;

    bool m_commitPage;
    bool m_showWelcome;
};

#endif //FIRST_RUN_WIZARD_H_
