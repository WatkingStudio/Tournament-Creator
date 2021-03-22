#include "include/eventsettings.h"
#include "ui_eventsettings.h"

EventSettings::EventSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventSettings)
{
    ui->setupUi(this);
    PopulateComboBox();
    SetupWidget();
}

EventSettings::~EventSettings()
{
    delete ui;
}

void EventSettings::SetupWidget()
{
    ui->WinLineEdit->setText("3");
    ui->DrawLineEdit->setText("1");
    ui->LossLineEdit->setText("0");
    ui->MostSportingLineEdit->setText("0");
    ui->BestPaintedLineEdit->setText("0");
    ui->NumberOfRoundsLineEdit->setText("6");
    ui->SeededPlayersCheckBox->setChecked(false);
    ui->FirstTiebreakerComboBox->setCurrentIndex(1);
    ui->SecondTiebreakerComboBox->setCurrentIndex(2);
    ui->ThirdTiebreakerComboBox->setCurrentIndex(3);
    ui->FourthTieBreakerComboBox->setCurrentIndex(4);
}

void EventSettings::PopulateComboBox()
{
    ui->FirstTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    ui->FirstTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    ui->FirstTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    ui->FirstTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    ui->FirstTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    ui->SecondTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    ui->SecondTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    ui->SecondTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    ui->SecondTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    ui->SecondTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    ui->ThirdTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    ui->ThirdTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    ui->ThirdTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    ui->ThirdTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    ui->ThirdTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    ui->FourthTieBreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    ui->FourthTieBreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    ui->FourthTieBreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    ui->FourthTieBreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    ui->FourthTieBreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());
}

void EventSettings::on_donePushButton_clicked()
{
    UtilDebug("Event Settings Done Clicked");
    EventSettingsData data = {
        ui->WinLineEdit->text().toInt(),
        ui->DrawLineEdit->text().toInt(),
        ui->LossLineEdit->text().toInt(),
        ui->MostSportingLineEdit->text().toInt(),
        ui->BestPaintedLineEdit->text().toInt(),
        ui->SeededPlayersCheckBox->isChecked(),
        ui->NumberOfRoundsLineEdit->text().toInt(),
        ui->FirstTiebreakerComboBox->currentText().toStdString(),
        ui->SecondTiebreakerComboBox->currentText().toStdString(),
        ui->ThirdTiebreakerComboBox->currentText().toStdString(),
        ui->FourthTieBreakerComboBox->currentText().toStdString()
    };
    emit SettingsComplete(data);
    this->hide();
}
