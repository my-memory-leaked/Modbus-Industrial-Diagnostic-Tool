#include <AumaTestGUI.hpp>
#include <ui_AumaTestGUI.h>
#include <ApplicationConstant.hpp>

AumaTestGUI::AumaTestGUI(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AumaTestGUI)
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::AUMA_TEST_GUI_NAME);
}

AumaTestGUI::~AumaTestGUI()
{
}
