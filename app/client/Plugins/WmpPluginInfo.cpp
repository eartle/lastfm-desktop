
#include <QStringList>

#include "WmpPluginInfo.h"

WmpPluginInfo::WmpPluginInfo( QObject* parent )
    :IPluginInfo( parent )
{}

Version
WmpPluginInfo::version() const
{
    return Version( 2, 1, 0, 8 );
}

QString
WmpPluginInfo::name() const
{
    return "Windows Media Player";
}

QString
WmpPluginInfo::displayName() const
{
    return QString( "MPlayer2" );
}

QString
WmpPluginInfo::processName() const
{
    return QString( "wmplayer.exe" );
}

QString
WmpPluginInfo::id() const
{
    return "wmp";
}

bool
WmpPluginInfo::isAppInstalled() const
{
    return QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\MediaPlayer\\Preferences", QSettings::NativeFormat).contains("FirstRun");
}

IPluginInfo::BootstrapType
WmpPluginInfo::bootstrapType() const
{
    return PluginBootstrap;
}

QString
WmpPluginInfo::pluginInstaller() const
{
    return "WmpPluginSetup_2.1.0.8.exe";
}
