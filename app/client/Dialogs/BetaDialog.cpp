
#include <lastfm/User.h>

#include "lib/unicorn/DesktopServices.h"

#include "../Application.h"

#include "BetaDialog.h"
#include "ui_BetaDialog.h"

BetaDialog::BetaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BetaDialog)
{
    ui->setupUi(this);

    setAttribute( Qt::WA_DeleteOnClose );

    ui->description->setText( "This is a beta version of the new Last.fm Desktop App. Please be gentle!" );

    if ( aApp->currentSession()->user().type() == lastfm::User::TypeStaff )
    {
        ui->feedback->setText( "If you've' noticed a problem, create a Jira issue and we'll fix it immediately. Thanks!" );

    }
    else
    {
        ui->createIssue->setText( tr( "Visit Audioscrobbler Beta group" ) );
        ui->feedback->setText( "If you've noticed a problem, please visit our group and leave us some feedback. Thanks!" );
    }

    connect( ui->createIssue, SIGNAL(clicked()), SLOT(createIssue()) );
}

void
BetaDialog::createIssue()
{
    if ( aApp->currentSession()->user().type() == lastfm::User::TypeStaff )
        unicorn::DesktopServices::openUrl( QUrl( "https://jira.last.fm/secure/CreateIssue.jspa?pid=10011&issuetype=1&Create=Create" ) );
    else
        unicorn::DesktopServices::openUrl( QUrl( "http://www.last.fm/group/Audioscrobbler+Beta" ) );
}

BetaDialog::~BetaDialog()
{
    delete ui;
}

