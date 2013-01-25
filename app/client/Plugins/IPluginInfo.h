#ifndef PLUGIN_INFO_H_
#define PLUGIN_INFO_H_

#include <QString>
#include <QSettings>

#ifdef WIN32
#include <windows.h>
#include <Shlobj.h>
#endif

class Version
{
public:
    Version(): m_valid( false ){}

    Version( unsigned int major, unsigned int minor = 0, unsigned int build = 0, unsigned int revision = 0 )
    :m_major(major), m_minor(minor), m_build(build), m_revision(revision), m_valid( true ){}

    QString toString() const;
    static Version fromString( const QString& string );

    bool isValid() const { return m_valid; }

    bool operator !=( const Version& that ) const{ return !(*this == that); }
    bool operator ==( const Version& that ) const
    {
        if( !m_valid || !that.m_valid ) return true;

        if( m_major != that.m_major ) return false;
        if( m_minor != that.m_minor ) return false;
        if( m_build != that.m_build ) return false;
        if( m_revision != that.m_revision ) return false;
        
        return true;
    }

    bool operator <( const Version& that ) const { return !(*this == that || *this > that ); }
    bool operator >( const Version& that ) const
    {
        if( !m_valid || !that.m_valid ) return true;

        if( m_major > that.m_major ) return true;
        if( m_minor > that.m_minor ) return true;
        if( m_build > that.m_build ) return true;
        if( m_revision > that.m_revision ) return true;
        
        return false;
    }

private:
    unsigned int m_major, m_minor, m_build, m_revision;
    bool m_valid;
};


class IPluginInfo : public QObject
{
    Q_OBJECT
public:
    IPluginInfo( QObject* parent = 0);

    enum BootstrapType{ NoBootstrap = 0, ClientBootstrap, PluginBootstrap };

    bool install() const;

    virtual QString name() const = 0;

    virtual Version version() const = 0;
    Version installedVersion() const;

    virtual QString id() const = 0;
    virtual BootstrapType bootstrapType() const = 0;

    virtual bool isAppInstalled() const = 0;

    virtual QString processName() const = 0;

    // DisplayName string value as found in the HKEY_LM/Software/Microsoft/CurrentVersion/Uninstall/{GUID}/
    virtual QString displayName() const = 0;

    virtual QString pluginInstaller() const = 0;

    static BOOL isWow64();

    virtual bool isInstalled() const;
    bool canBootstrap() const;

    void setVerbose( bool verbose );

public slots:
    void doInstall();
    void install( bool install );

protected:
    QString programFilesX86() const;
    QString programFiles64() const;

private:
    bool m_install;
    bool m_verbose;
};

#endif //PLUGIN_INFO_H_
