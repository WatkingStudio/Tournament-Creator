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
    emit SettingsComplete(ui->winLineEdit->text().toInt(), ui->drawLineEdit->text().toInt(), ui->lossLineEdit->text().toInt(), ui->mostSportingLineEdit->text().toInt(), ui->bestPaintedLineEdit->text().toInt(), ui->seededPlayersCheckBox->isChecked(), ui->numberOfRoundsLineEdit->text().toInt(), ui->firstTiebreakerComboBox->currentText().toStdString(), ui->secondTiebreakerComboBox->currentText().toStdString(), ui->thirdTiebreakerComboBox->currentText().toStdString(), ui->fourthTieBreakerComboBox->currentText().toStdString());
    this->hide();
}
