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
    ui->winLineEdit->setText("3");
    ui->drawLineEdit->setText("1");
    ui->lossLineEdit->setText("0");
    ui->mostSportingLineEdit->setText("0");
    ui->bestPaintedLineEdit->setText("0");
    ui->numberOfRoundsLineEdit->setText("6");
    ui->seededPlayersCheckBox->setChecked(false);
    ui->firstTiebreakerComboBox->setCurrentIndex(1);
    ui->secondTiebreakerComboBox->setCurrentIndex(2);
    ui->thirdTiebreakerComboBox->setCurrentIndex(3);
    ui->fourthTieBreakerComboBox->setCurrentIndex(4);
}

void EventSettings::PopulateComboBox()
{
    ui->firstTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    ui->firstTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    ui->firstTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    ui->firstTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    ui->firstTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    ui->secondTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    ui->secondTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    ui->secondTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    ui->secondTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    ui->secondTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    ui->thirdTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    ui->thirdTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    ui->thirdTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    ui->thirdTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    ui->thirdTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    ui->fourthTieBreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    ui->fourthTieBreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    ui->fourthTieBreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    ui->fourthTieBreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    ui->fourthTieBreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());
}

void EventSettings::on_donePushButton_clicked()
{
    UtilDebug("Event Settings Done Clicked");
    emit SettingsComplete(ui->winLineEdit->text().toInt(), ui->drawLineEdit->text().toInt(), ui->lossLineEdit->text().toInt(), ui->mostSportingLineEdit->text().toInt(), ui->bestPaintedLineEdit->text().toInt(), ui->seededPlayersCheckBox->isChecked(), ui->numberOfRoundsLineEdit->text().toInt(), ui->firstTiebreakerComboBox->currentText().toStdString(), ui->secondTiebreakerComboBox->currentText().toStdString(), ui->thirdTiebreakerComboBox->currentText().toStdString(), ui->fourthTieBreakerComboBox->currentText().toStdString());
    this->hide();
}
