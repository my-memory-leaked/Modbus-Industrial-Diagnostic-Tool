#include <AumaTestGUI.hpp>
#include <ui_AumaTestGUI.h>
#include <ApplicationConstant.hpp>
#include <Logger.hpp>

#include <ActuatorResponseValidation.hpp>

static auto* logger = &Singleton<Logger>::GetInstance();

AumaTestGUI::AumaTestGUI(std::shared_ptr<TestInterface> &testInterface, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AumaTestGUI),
      _testInterface(testInterface)
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::AUMA_TEST_GUI_NAME);
    logger->LogInfo(TAG, "Opened AumaTestGUI");
}

AumaTestGUI::~AumaTestGUI()
{
}

void AumaTestGUI::accept()
{
    QString radioButtonText = ui->TestButtonGroup->checkedButton()->text();
    logger->LogInfo(TAG, "Chosen " + radioButtonText + " test");

    if(radioButtonText == "Actuator Response Validation")
    {
        _testInterface = std::make_shared<ActuatorResponseValidation>();
    }
    else if(radioButtonText == "Process Controller Operation and Stability")
    {
        _testInterface = std::make_shared<ActuatorResponseValidation>();
    }
    else if(radioButtonText == "Firmware Detail Verification")
    {
        _testInterface = std::make_shared<ActuatorResponseValidation>();
    }
    else
    {
        logger->LogCritical(TAG, "Test not implemented!");
    }

    QDialog::accept();
}
