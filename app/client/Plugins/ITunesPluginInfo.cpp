
#include <QStringList>

#include "ITunesPluginInfo.h"

ITunesPluginInfo::ITunesPluginInfo( QObject* parent )
    :IPluginInfo( parent )
{}

Version
ITunesPluginInfo::version() const
{
    return Version( 5, 0, 5, 4 );
}

QString
ITunesPluginInfo::name() const
{
    return "iTunes";
}

QString
ITunesPluginInfo::displayName() const
{
    return QString( "iTunes" );
}

QString
ITunesPluginInfo::processName() const
{
    return QString( "iTunes.exe" );
}

QString
ITunesPluginInfo::id() const
{
#ifdef Q_OS_WIN32
    return "itw";
#elif
    return "osx";
#endif
}

bool
ITunesPluginInfo::isAppInstalled() const
{
#ifdef Q_OS_WIN32
    return QSettings("HKEY_CURRENT_USER\\Software\\Apple Computer, Inc.\\iTunes", QSettings::NativeFormat).contains("SM Shortcut Installed");
#elif
    return true;
#endif
}

IPluginInfo::BootstrapType
ITunesPluginInfo::bootstrapType() const
{
    return ClientBootstrap;
}

QString
ITunesPluginInfo::pluginInstaller() const
{
    return "iTunesPluginWinSetup_5.0.5.4.exe";
}

