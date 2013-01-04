#include "UnicornApplication.h"
#import <Foundation/NSAppleEventDescriptor.h>
#import <Foundation/NSAppleEventManager.h>
#import <Foundation/NSObject.h>

void 
unicorn::Application::setOpenApplicationEventHandler()
{
    NSAppleEventManager *appleEventManager = [NSAppleEventManager sharedAppleEventManager];
    [appleEventManager setEventHandler:NULL andSelector:NULL forEventClass:kCoreEventClass andEventID:kAEReopenApplication];
}

void
unicorn::Application::setGetURLEventHandler()
{
    NSAppleEventManager *appleEventManager = [NSAppleEventManager sharedAppleEventManager];
    [appleEventManager setEventHandler:NULL andSelector:NULL forEventClass:kInternetEventClass andEventID:kAEGetURL];
}

void
unicorn::Application::macTranslate( const QString& lang )
{
    NSString* langString = [NSString stringWithCharacters:(const unichar *)lang.unicode() length:(NSUInteger)lang.length() ];

    [[NSUserDefaults standardUserDefaults] setObject:[NSArray arrayWithObject:langString] forKey:@"AppleLanguages"];
}

