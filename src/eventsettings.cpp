#include "include/eventsettings.h"
#include "ui_eventsettings.h"
#include <QMessageBox>

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

bool EventSettings::CheckUniqueTiebreakers() const
{
    if(m_Ui->FirstTiebreakerComboBox->currentText() == m_Ui->SecondTiebreakerComboBox->currentText()
            || m_Ui->FirstTiebreakerComboBox->currentText() == m_Ui->ThirdTiebreakerComboBox->currentText()
            || m_Ui->FirstTiebreakerComboBox->currentText() == m_Ui->FourthTieBreakerComboBox->currentText())
    {
        return false;
    }
    else if(m_Ui->SecondTiebreakerComboBox->currentText() == m_Ui->ThirdTiebreakerComboBox->currentText()
            || m_Ui->SecondTiebreakerComboBox->currentText() == m_Ui->FourthTieBreakerComboBox->currentText())
    {
        return false;
    }
    else if(m_Ui->ThirdTiebreakerComboBox->currentText() == m_Ui->FourthTieBreakerComboBox->currentText())
    {
        return false;
    }

    return true;
}

void EventSettings::on_DonePushButton_clicked()
{
    UtilDebug("Event Settings Done Clicked");

    if(CheckUniqueTiebreakers())
    {
        EventSettingsData data = {
            m_Ui->WinSpinBox->value(),
            m_Ui->DrawSpinBox->value(),
            m_Ui->LossSpinBox->value(),
            m_Ui->MostSportingSpinBox->value(),
            m_Ui->BestPaintedSpinBox->value(),
            m_Ui->SeededPlayersCheckBox->isChecked(),
            m_Ui->NumberOfRoundsSpinBox->value(),
            m_Ui->FirstTiebreakerComboBox->currentText().toStdString(),
            m_Ui->SecondTiebreakerComboBox->currentText().toStdString(),
            m_Ui->ThirdTiebreakerComboBox->currentText().toStdString(),
            m_Ui->FourthTieBreakerComboBox->currentText().toStdString()
        };
        emit SettingsComplete(data);
        this->hide();
    }
    else
    {
        QMessageBox::warning(this, "Duplicate Tiebreakers", "Some of the Tiebreakers set are duplicate. Ensure that they are all unique.");
    }
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
    m_Ui->WinSpinBox->setValue(3);
    m_Ui->DrawSpinBox->setValue(1);
    m_Ui->LossSpinBox->setValue(0);
    m_Ui->MostSportingSpinBox->setValue(0);
    m_Ui->BestPaintedSpinBox->setValue(0);
    m_Ui->NumberOfRoundsSpinBox->setValue(6);
    m_Ui->SeededPlayersCheckBox->setChecked(false);
    m_Ui->FirstTiebreakerComboBox->setCurrentIndex(1);
    m_Ui->SecondTiebreakerComboBox->setCurrentIndex(2);
    m_Ui->ThirdTiebreakerComboBox->setCurrentIndex(3);
    m_Ui->FourthTieBreakerComboBox->setCurrentIndex(4);
}
