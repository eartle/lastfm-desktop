#include "ProxyDialog.h"
#include "ui_ProxyDialog.h"

ProxyDialog::ProxyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProxyDialog)
{
    ui->setupUi(this);

    connect( this, SIGNAL(accepted()), SLOT(onAccepted()));

    setFixedSize( width(), height() );
}

ProxyDialog::~ProxyDialog()
{
    delete ui;
}

void
ProxyDialog::onAccepted()
{
    ui->proxySettings->save();
}
