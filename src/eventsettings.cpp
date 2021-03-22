#include "include/eventsettings.h"
#include "ui_eventsettings.h"

EventSettings::EventSettings(QWidget *parent) :
    QWidget(parent),
    m_Ui(new Ui::EventSettings)
{
    m_Ui->setupUi(this);
    PopulateComboBox();
    SetupWidget();
}

EventSettings::~EventSettings()
{
    delete m_Ui;
}

void EventSettings::on_DonePushButton_clicked()
{
    UtilDebug("Event Settings Done Clicked");
    EventSettingsData data = {
        m_Ui->WinLineEdit->text().toInt(),
        m_Ui->DrawLineEdit->text().toInt(),
        m_Ui->LossLineEdit->text().toInt(),
        m_Ui->MostSportingLineEdit->text().toInt(),
        m_Ui->BestPaintedLineEdit->text().toInt(),
        m_Ui->SeededPlayersCheckBox->isChecked(),
        m_Ui->NumberOfRoundsLineEdit->text().toInt(),
        m_Ui->FirstTiebreakerComboBox->currentText().toStdString(),
        m_Ui->SecondTiebreakerComboBox->currentText().toStdString(),
        m_Ui->ThirdTiebreakerComboBox->currentText().toStdString(),
        m_Ui->FourthTieBreakerComboBox->currentText().toStdString()
    };
    emit SettingsComplete(data);
    this->hide();
}

void EventSettings::PopulateComboBox()
{
    m_Ui->FirstTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    m_Ui->FirstTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    m_Ui->FirstTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    m_Ui->FirstTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    m_Ui->FirstTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    m_Ui->SecondTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    m_Ui->SecondTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    m_Ui->SecondTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    m_Ui->SecondTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    m_Ui->SecondTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    m_Ui->ThirdTiebreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    m_Ui->ThirdTiebreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    m_Ui->ThirdTiebreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    m_Ui->ThirdTiebreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    m_Ui->ThirdTiebreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());

    m_Ui->FourthTieBreakerComboBox->addItem(Tiebreak::NoneString->c_str());
    m_Ui->FourthTieBreakerComboBox->addItem(Tiebreak::VPTotalString->c_str());
    m_Ui->FourthTieBreakerComboBox->addItem(Tiebreak::VPDiffString->c_str());
    m_Ui->FourthTieBreakerComboBox->addItem(Tiebreak::MostSportingString->c_str());
    m_Ui->FourthTieBreakerComboBox->addItem(Tiebreak::BestPaintedString->c_str());
}

void EventSettings::SetupWidget()
{
    m_Ui->WinLineEdit->setText("3");
    m_Ui->DrawLineEdit->setText("1");
    m_Ui->LossLineEdit->setText("0");
    m_Ui->MostSportingLineEdit->setText("0");
    m_Ui->BestPaintedLineEdit->setText("0");
    m_Ui->NumberOfRoundsLineEdit->setText("6");
    m_Ui->SeededPlayersCheckBox->setChecked(false);
    m_Ui->FirstTiebreakerComboBox->setCurrentIndex(1);
    m_Ui->SecondTiebreakerComboBox->setCurrentIndex(2);
    m_Ui->ThirdTiebreakerComboBox->setCurrentIndex(3);
    m_Ui->FourthTieBreakerComboBox->setCurrentIndex(4);
}
