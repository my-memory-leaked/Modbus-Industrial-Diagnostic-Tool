#include <AumaTestGUI.hpp>
#include <ui_AumaTestGUI.h>
#include <ApplicationConstant.hpp>
#include <Logger.hpp>

#include <ActuatorResponseValidation.hpp>
#include <ProcessControllerOperationAndStability.hpp>
#include <FirmwareDetailVerification.hpp>

#include <QMessageBox>

static auto* logger = &Singleton<Logger>::GetInstance();

AumaTestGUI::AumaTestGUI(std::shared_ptr<TestInterface> &testInterface, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AumaTestGUI),
      _testInterface(testInterface)
{
    ui->setupUi(this);
    setWindowTitle(ApplicationConstant::AUMA_TEST_GUI_NAME);
    logger->LogInfo(TAG, "Opened AumaTestGUI");


    QString appDirPath = QCoreApplication::applicationDirPath() + "/Images/InfoIcon.png";
    QPixmap aumaLogoPixmap(appDirPath);
    if (!aumaLogoPixmap.isNull())
    {
        /* Scale logo */
        QPixmap scaledPixmap = aumaLogoPixmap.scaled(ui->FirmwareDetailInfoButton->size(),
                                                     Qt::KeepAspectRatio, Qt::SmoothTransformation);

        ui->FirmwareDetailInfoButton->setIcon(scaledPixmap);
        ui->FCOSInfoButton->setIcon(scaledPixmap);
        ui->ARVInfoButton->setIcon(scaledPixmap);
    }
    connect(ui->FirmwareDetailInfoButton, &QPushButton::clicked, this, &AumaTestGUI::onButtonClicked);
    connect(ui->FCOSInfoButton, &QPushButton::clicked, this, &AumaTestGUI::onButtonClicked);
    connect(ui->ARVInfoButton, &QPushButton::clicked, this, &AumaTestGUI::onButtonClicked);

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
        _testInterface = std::make_shared<ProcessControllerOperationAndStability>();
    }
    else if(radioButtonText == "Firmware Detail Verification")
    {
        _testInterface = std::make_shared<FirmwareDetailVerification>();
    }
    else
    {
        logger->LogCritical(TAG, "Test not implemented!");
    }

    QDialog::accept();
}


void AumaTestGUI::onButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString infoText;
        if (button == ui->FirmwareDetailInfoButton) {
            infoText = "The primary goal of this test is to ensure that the system accurately displays and operates with the correct firmware version, language settings, device ID, and operating mode. This is crucial for maintaining the integrity and functionality of the device or software system.";
        } else if (button == ui->FCOSInfoButton) {
            infoText = "Information about FCOS";
        } else if (button == ui->ARVInfoButton) {
            infoText = "Information about ARV";
        }

        QMessageBox::information(this, "Information", infoText);
    }
}
