
#include <QSettings>
#include <QStringList>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

#include <windows.h>
#include <Shellapi.h>
#include <Wtsapi32.h>

#include "lib/unicorn/QMessageBoxBuilder.h"

#include "lib/unicorn/UnicornSettings.h"

#include "../Dialogs/CloseAppsDialog.h"

#include "IPluginInfo.h"
#include "KillProcess.h"

QString
Version::toString() const
{
    return QString( "%1.%2.%3.%4" ).arg( QString::number( m_major ),
                                         QString::number( m_minor ),
                                         QString::number( m_build ),
                                         QString::number( m_revision ) );
}

Version
Version::fromString( const QString& string )
{
    QStringList versionList = string.split( "." );

    int major = versionList.count() > 0 ? versionList.at( 0 ).toUInt() : 0;
    int minor = versionList.count() > 1 ? versionList.at( 1 ).toUInt() : 0;
    int build = versionList.count() > 2 ? versionList.at( 2 ).toUInt() : 0;
    int revision = versionList.count() > 3 ? versionList.at( 3 ).toUInt() : 0;

    return Version( major, minor, build, revision );
}

IPluginInfo::IPluginInfo( QObject* parent )
    :QObject( parent ), m_install( false )
{}

void
IPluginInfo::doInstall()
{
    QList<IPluginInfo*> plugins;
    plugins << this;

    CloseAppsDialog* closeApps = new CloseAppsDialog( plugins, 0 );

    if ( closeApps->result() != QDialog::Accepted )
        closeApps->exec();
    else
        closeApps->deleteLater();

    if ( closeApps->result() == QDialog::Accepted )
    {
        QString installer = QString( "\"%1\"" ).arg( QCoreApplication::applicationDirPath() + "/plugins/" + pluginInstaller() );
        qDebug() << installer;
        QProcess* installerProcess = new QProcess( this );
        installerProcess->start( installer, QStringList() << "/SILENT" );
        bool finished = installerProcess->waitForFinished( -1 );
        qDebug() << finished << installerProcess->error() << installerProcess->errorString();

    }
    else
    {
        // The user didn't close their media players
        QMessageBoxBuilder( 0 ).setTitle( tr( "The %1 plugin hasn't been installed" ).arg( name() ) )
                .setIcon( QMessageBox::Warning )
                .setText( tr( "You didn't close %1 so its plugin hasn't been installed." ).arg( name() ) )
                .setButtons( QMessageBox::Ok )
                .exec();
    }
}

bool
IPluginInfo::install() const
{
    return m_install;
}


void
IPluginInfo::install( bool install )
{
    m_install = install;
}

BOOL
IPluginInfo::isWow64()
{
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    void* fnIsWow64Process = GetProcAddress(
    GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!IsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
}


bool
IPluginInfo::isInstalled() const
{
    QSettings settings( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Last.fm\\Client\\Plugins", QSettings::NativeFormat );
    return settings.childGroups().contains( id() );
}

Version
IPluginInfo::installedVersion() const
{
    QSettings settings( "HKEY_LOCAL_MACHINE\\SOFTWARE\\Last.fm\\Client\\Plugins\\" + id(), QSettings::NativeFormat );
    return Version::fromString( settings.value( "Version", "0.0.0.0" ).toString() );
}

bool
IPluginInfo::canBootstrap() const
{
    return bootstrapType() != NoBootstrap;
}

QString
IPluginInfo::programFilesX86() const
{
    return QString( getenv( "ProgramFiles(x86)" ) );
}

QString
IPluginInfo::programFiles64() const
{
    return QString( getenv( "ProgramW6432" ) );
}

