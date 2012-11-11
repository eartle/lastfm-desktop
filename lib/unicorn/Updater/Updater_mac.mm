
#include <QStringList>

#include <../UnicornApplication.h>
#include <../UnicornSettings.h>

#include "Updater.h"

#include <AppKit/NSNibDeclarations.h>
#include <Foundation/Foundation.h>
#include <Foundation/NSURL.h>

#include <Sparkle/Sparkle.h>
#include <Sparkle/SUUpdater.h>

@interface UpdaterDelegate : NSObject {
}
- (void)updater:(SUUpdater *)updater willInstallUpdate:(SUAppcastItem *)update;
- (void)updaterWillRelaunchApplication:(SUUpdater *)updater;
@end

@implementation UpdaterDelegate

- (void)updater:(SUUpdater *)updater willInstallUpdate:(SUAppcastItem *)update
{
    Q_UNUSED(updater)
    Q_UNUSED(update)
}

- (void) updaterWillRelaunchApplication:(SUUpdater *)updater
{
    Q_UNUSED(updater)

    static_cast<unicorn::Application*>(qApp)->hideDockIcon( unicorn::Settings().value( SETTING_SHOW_WHERE, 0 ).toInt() == 2 );
}

@end

UpdaterDelegate* g_Delegate;

unicorn::Updater::Updater(QObject *parent) :
    QObject(parent)
{
    SUUpdater* updater = [SUUpdater sharedUpdater];
    g_Delegate = [UpdaterDelegate alloc];
    [updater setDelegate:g_Delegate];

    if ( qApp->arguments().contains( "--update" ) )
        [updater setFeedURL:[NSURL URLWithString:@"http://users.last.fm/~michael/updates_mac.xml"]];
    else if ( qApp->arguments().contains( "--update-static" ) )
        [updater setFeedURL:[NSURL URLWithString:@"http://static.last.fm/client/Mac/updates.xml"]];
    else
        [updater setFeedURL:[NSURL URLWithString:@"http://cdn.last.fm/client/Mac/updates.xml"]];
}

void
unicorn::Updater::checkForUpdates()
{
    [[SUUpdater sharedUpdater] checkForUpdates:0];
}

unicorn::Updater::~Updater()
{
    // do nothing - included here for header compatibility with windows version
}
