#ifndef USERUI_H
#define USERUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UserUI; }
QT_END_NAMESPACE

class UserUI : public QMainWindow
{
    Q_OBJECT

public:
    UserUI(QWidget *parent = nullptr);
    ~UserUI();

private:
    Ui::UserUI *ui;
};
#endif // USERUI_H
