#pragma once

#include <memory>
#include <QDialog>

namespace Ui {
class FirmwareDetailGUI;
}

class FirmwareDetailGUI : public QDialog
{
    Q_OBJECT

public:
    explicit FirmwareDetailGUI(QWidget *parent = nullptr);
    ~FirmwareDetailGUI();

    void SetProgressBar(int value);
    void SetFirmwareVersionLabel(QString string);
    void SetLanguageLabel(QString string);
    void SetServiceInterfaceInfo(QPair<QString, int> info);
private:
    std::unique_ptr<Ui::FirmwareDetailGUI> ui;
};

