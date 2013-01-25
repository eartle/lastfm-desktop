#ifndef PROXYDIALOG_H
#define PROXYDIALOG_H

#include "lib/DllExportMacro.h"

#include <QDialog>

namespace Ui { class ProxyDialog; }

namespace unicorn
{

class UNICORN_DLLEXPORT ProxyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProxyDialog(QWidget *parent = 0);
    ~ProxyDialog();

private slots:
    void onAccepted();
    
private:
    Ui::ProxyDialog *ui;
};

}

#endif // PROXYDIALOG_H
