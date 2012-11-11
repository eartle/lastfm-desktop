#include "CloseAppsDialog.h"

QString NSStringToQString(const NSString *nsstr)
{
    NSRange range;
    range.location = 0;
    range.length = [nsstr length];

    unichar *chars = new unichar[range.length];
    [nsstr getCharacters:chars range:range];
    QString result = QString::fromUtf16(chars, range.length);
    delete[] chars;
    return result;
}

QStringList
CloseAppsDialog::runningApps()
{
    // we only ever test for iTunes on Mac

    QStringList apps;

    // make sure iTunes isn't running
    NSArray* runningApps = [[NSWorkspace sharedWorkspace] runningApplications];

    for ( unsigned int i = 0 ; i < [runningApps count] ; ++i )
    {
        if ( [[[runningApps objectAtIndex:i] bundleIdentifier] isEqualToString:@"com.apple.iTunes"] )
        {
            NSString* appName = [[runningApps objectAtIndex:i] localizedName];
            apps << NSStringToQString( appName );
            break;
        }
    }

    return apps;
}
