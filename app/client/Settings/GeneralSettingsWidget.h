#ifndef GENERALSETTINGSWIDGET_H
#define GENERALSETTINGSWIDGET_H

#include "SettingsWidget.h"

namespace Ui {
    class GeneralSettingsWidget;
}

class GeneralSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    GeneralSettingsWidget( QWidget* parent = 0 );

private:
    void saveSettings();
    void populateLanguages();

private:
    Ui::GeneralSettingsWidget* ui;
};

#endif // GENERALSETTINGSWIDGET_H
