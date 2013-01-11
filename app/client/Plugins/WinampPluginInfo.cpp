
#include <QStringList>

#include "WinampPluginInfo.h"

WinampPluginInfo::WinampPluginInfo( QObject* parent )
    :IPluginInfo( parent )
{}

Version
WinampPluginInfo::version() const
{
    return Version( 2, 1, 0, 11 );
}

QString
WinampPluginInfo::name() const
{
    return "Winamp";
}

QString
WinampPluginInfo::displayName() const
{
    return QString( "MPlayer2" );
}

QString
WinampPluginInfo::processName() const
{
    return QString( "winamp.exe" );
}

QString
WinampPluginInfo::id() const
{
    return "wa2";
}

bool
WinampPluginInfo::isAppInstalled() const
{
    return QSettings("HKEY_CURRENT_USER\\Software\\Winamp", QSettings::NativeFormat).contains(".");
}

IPluginInfo::BootstrapType
WinampPluginInfo::bootstrapType() const
{
    return PluginBootstrap;
}

QString
WinampPluginInfo::pluginInstaller() const
{
    return "WinampPluginSetup_2.1.0.11.exe";
}
