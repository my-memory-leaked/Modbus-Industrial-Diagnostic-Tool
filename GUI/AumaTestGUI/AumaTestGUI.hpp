#ifndef AUMATESTGUI_HPP
#define AUMATESTGUI_HPP

#include <QDialog>

#include <memory>

namespace Ui {
class AumaTestGUI;
}

class AumaTestGUI : public QDialog
{
    Q_OBJECT

public:
    explicit AumaTestGUI(QWidget *parent = nullptr);
    ~AumaTestGUI();

private:
    std::unique_ptr<Ui::AumaTestGUI> ui;
};

#endif // AUMATESTGUI_HPP
