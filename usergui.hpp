#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UserGUI; }
QT_END_NAMESPACE

class UserGUI : public QMainWindow
{
    Q_OBJECT

public:
    UserGUI(QWidget *parent = nullptr);
    ~UserGUI();

private:
    Ui::UserGUI *ui;
};
