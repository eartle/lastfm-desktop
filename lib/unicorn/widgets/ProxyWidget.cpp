
#include <QNetworkProxy>

#include <lastfm/NetworkAccessManager.h>

#include "lib/unicorn/UnicornSettings.h"

#include "ProxyWidget.h"
#include "ui_ProxyWidget.h"

unicorn::ProxyWidget::ProxyWidget(QWidget *parent) :
    QWidget(parent), ui( new Ui::ProxyWidget)
{
    ui->setupUi( this );

    unicorn::AppSettings appSettings;

    QStringList proxyTypes;
    proxyTypes << tr( "Auto-detect" ) << tr( "No-proxy" ) << tr("HTTP") << tr("SOCKS5");
    ui->proxyType->addItems( proxyTypes );
    ui->proxyType->setCurrentIndex( appSettings.value( "proxyType", 0 ).toInt() );
    ui->proxyHost->setText( appSettings.value( "proxyHost", "" ).toString() );
    ui->proxyPort->setText( appSettings.value( "proxyPort", "" ).toString() );
    ui->proxyUsername->setText( appSettings.value( "proxyUsername", "" ).toString() );
    ui->proxyPassword->setText( appSettings.value( "proxyPassword", "" ).toString() );

    connect( ui->proxyType, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
    connect( ui->proxyHost, SIGNAL(textChanged(QString)), SIGNAL(changed()));
    connect( ui->proxyPort, SIGNAL(textChanged(QString)), SIGNAL(changed()));
    connect( ui->proxyUsername, SIGNAL(textChanged(QString)), SIGNAL(changed()));
    connect( ui->proxyPassword, SIGNAL(textChanged(QString)), SIGNAL(changed()));

    connect( this, SIGNAL(changed()), SLOT(onChanged()) );

    onChanged();
}

unicorn::ProxyWidget::~ProxyWidget()
{
    delete ui;
}

void
unicorn::ProxyWidget::onChanged()
{
    bool enabled = ui->proxyType->currentIndex() > 1;
    ui->proxyHost->setEnabled( enabled );
    ui->proxyPort->setEnabled( enabled );
    ui->proxyUsername->setEnabled( enabled );
    ui->proxyPassword->setEnabled( enabled );
}

void
unicorn::ProxyWidget::save()
{
    unicorn::AppSettings appSettings;
    int proxyType = appSettings.value( "proxyType", 0 ).toInt();
    QString proxyHost = appSettings.value( "proxyHost", "" ).toString();
    QString proxyPort = appSettings.value( "proxyPort", "" ).toString();
    QString proxyUsername = appSettings.value( "proxyUsername", "" ).toString();
    QString proxyPassword = appSettings.value( "proxyPassword", "" ).toString();

    if ( proxyType != ui->proxyType->currentIndex()
         || proxyHost != ui->proxyHost->text()
         || proxyPort != ui->proxyPort->text()
         || proxyUsername != ui->proxyUsername->text()
         || proxyPassword != ui->proxyPassword->text() )
    {
        // one of the proxy settings has changed

        // save them
        appSettings.setValue( "proxyType", ui->proxyType->currentIndex() );
        appSettings.setValue( "proxyHost", ui->proxyHost->text() );
        appSettings.setValue( "proxyPort", ui->proxyPort->text() );
        appSettings.setValue( "proxyUsername", ui->proxyUsername->text() );
        appSettings.setValue( "proxyPassword", ui->proxyPassword->text() );

        // set this new proxy
        QNetworkProxy::ProxyType type = QNetworkProxy::DefaultProxy;

        if ( ui->proxyType->currentIndex() == 1 )
            type = QNetworkProxy::NoProxy;
        else if ( ui->proxyType->currentIndex() == 2 )
            type = QNetworkProxy::HttpProxy;
        else if ( ui->proxyType->currentIndex() == 3 )
            type = QNetworkProxy::Socks5Proxy;

        QNetworkProxy proxy( type, ui->proxyHost->text(), ui->proxyPort->text().toInt(), ui->proxyUsername->text(), ui->proxyPassword->text() );
        lastfm::NetworkAccessManager* nam = qobject_cast<lastfm::NetworkAccessManager*>( lastfm::nam() );

        if ( nam )
            nam->setUserProxy( proxy );
    }
}
