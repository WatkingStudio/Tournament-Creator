#include "include/eventsettings.h"
#include "ui_eventsettings.h"

EventSettings::EventSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventSettings)
{
    ui->setupUi(this);
}

EventSettings::~EventSettings()
{
    delete ui;
}

void EventSettings::on_donePushButton_clicked()
{
    this->hide();
}
