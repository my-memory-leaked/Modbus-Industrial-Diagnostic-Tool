#include "userui.h"
#include "./ui_userui.h"

UserUI::UserUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserUI)
{
    ui->setupUi(this);
}

UserUI::~UserUI()
{
    delete ui;
}

