#include "configgui.h"
#include "ui_configgui.h"

ConfigGUI::ConfigGUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigGUI)
{
    ui->setupUi(this);
}

ConfigGUI::~ConfigGUI()
{
    delete ui;
}
