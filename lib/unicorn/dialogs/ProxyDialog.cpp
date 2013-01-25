#include "ProxyDialog.h"
#include "ui_ProxyDialog.h"

unicorn::ProxyDialog::ProxyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProxyDialog)
{
    ui->setupUi(this);

    connect( this, SIGNAL(accepted()), SLOT(onAccepted()));

    setFixedSize( width(), height() );
}

unicorn::ProxyDialog::~ProxyDialog()
{
    delete ui;
}

void
unicorn::ProxyDialog::onAccepted()
{
    ui->proxySettings->save();
}
