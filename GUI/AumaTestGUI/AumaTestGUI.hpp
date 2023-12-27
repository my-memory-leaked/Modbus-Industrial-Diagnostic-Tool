#ifndef AUMATESTGUI_HPP
#define AUMATESTGUI_HPP

#include <QDialog>
#include <TestInterface.hpp>
#include <memory>

namespace Ui {
class AumaTestGUI;
}

class AumaTestGUI : public QDialog
{
    Q_OBJECT

public:
    explicit AumaTestGUI(std::shared_ptr<TestInterface> &testInterface, QWidget *parent = nullptr);
    ~AumaTestGUI();

private slots:
    void accept() override;

private:
    std::unique_ptr<Ui::AumaTestGUI> ui;
    std::shared_ptr<TestInterface> &_testInterface;

    static constexpr const char TAG[] {"[AumaTestGUI]"};

};

#endif // AUMATESTGUI_HPP
