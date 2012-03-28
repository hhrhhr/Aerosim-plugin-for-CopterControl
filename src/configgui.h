#ifndef CONFIGGUI_H
#define CONFIGGUI_H

#include <QDialog>

namespace Ui {
class ConfigGUI;
}

class ConfigGUI : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigGUI(QWidget *parent = 0);
    ~ConfigGUI();
    
private:
    Ui::ConfigGUI *ui;
};

#endif // CONFIGGUI_H
