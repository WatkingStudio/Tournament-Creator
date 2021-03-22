#include "include/mainwindow.h"
#include "ui_mainwindow.h"

// This .cpp file is used to handle the tournament creator page

void MainWindow::LoadTournamentCreatorPage()
{
    UtilDebug("Load Tournament Creator Page");
    int i = 0;
    QString filePath;
    while(true)
    {
        filePath = *m_EventDirectory + *m_EventDefaultFileName + std::to_string(i++).c_str() + ".json";
        if(!FileExists(filePath))
        {
            *m_EventDataFileName = filePath.toStdString();
            break;
        }
    }
    CreateFile(m_EventDataFileName->c_str(), *m_EventDirectory);

    m_Ui->StackedWidget->setCurrentIndex(Pages::TOURNAMENT_CREATOR);
    m_Ui->PageTitleWidget->setText("Tournament Creator");

    *m_TournamentCreatorSelectedCol = -1;
    *m_TournamentCreatorSelectedRow = -1;

    UpdatePlayerTable();
}

void MainWindow::on_AddPlayerButton_clicked()
{
    UtilDebug("Add Player Clicked");
    if(m_Ui->PlayerNameInputLineEdit->displayText() != "")
    {
        UtilDebug("Add Player");

        std::string name = m_Ui->PlayerNameInputLineEdit->text().toStdString();
        uint seed = m_Ui->PlayerSeedInputLineEdit->text().toInt();

        m_TempPlayerList->emplace_back(name, seed);

        UpdatePlayerTable();
        m_Ui->PlayerNameInputLineEdit->clear();
        m_Ui->PlayerSeedInputLineEdit->clear();
    }
}

void MainWindow::on_BackTournamentCreatorButton_clicked()
{
    LoadStartPage();
}

void MainWindow::on_ContinueTournamentCreatorButton_clicked()
{
    UtilDebug("Tournament Creator Continue Button Clicked");
    if(m_TempPlayerList->size() > 0)
    {
        *m_MainPlayerList = *m_TempPlayerList;

        LoadMatchupsPage();
    }
    else
    {
        QMessageBox::warning(this, "Warning", "No Players Detected.");
    }

    if(m_EventSettingsWidget.isVisible())
    {
        m_EventSettingsWidget.hide();
    }

    SaveEventData();
}

void MainWindow::on_EventSettingsButton_clicked()
{
    UtilDebug("Event Settings Clicked");
    m_EventSettingsWidget.show();
}

void MainWindow::on_RemovePlayerButton_clicked()
{
    UtilDebug("Remove Player Clicked");
    if(*m_TournamentCreatorSelectedRow != -1)
    {
        m_Ui->TournamentCreatorPlayerTableWidget->removeRow(*m_TournamentCreatorSelectedRow);
        m_TempPlayerList->erase(m_TempPlayerList->begin() + *m_TournamentCreatorSelectedRow);
        *m_TournamentCreatorSelectedRow = -1;
    }
}

void MainWindow::on_SaveEventTournamentCreatorButton_clicked()
{
    UtilDebug("Tournament Creator Save Event Button Clicked");
    SaveEventData();
}

void MainWindow::ReceiveEventSettings(const EventSettingsData &eventSettingsData)
{
    UtilDebug("Event Settings Changed");

    if(eventSettingsData.winTPs > 0)
    {
        *m_WinValue = eventSettingsData.winTPs;
    }
    if(eventSettingsData.drawTPs > 0)
    {
        *m_DrawValue = eventSettingsData.drawTPs;
    }
    if(eventSettingsData.lossTPs > 0)
    {
        *m_LossValue = eventSettingsData.lossTPs;
    }
    if(eventSettingsData.mostSportingTPs >= 0)
    {
        *m_MostSportingValue = eventSettingsData.mostSportingTPs;
    }
    if(eventSettingsData.bestPaintedTPs >= 0)
    {
        *m_BestPaintedValue = eventSettingsData.bestPaintedTPs;
    }
    *m_UsingSeed = eventSettingsData.usingSeeded;
    if(eventSettingsData.numberOfRounds > 0)
    {
        *m_NumberOfRounds = eventSettingsData.numberOfRounds;
    }

    *m_FirstTiebreaker = Tiebreak::StringToTiebreaker(eventSettingsData.firstTiebreaker);
    *m_SecondTiebreaker = Tiebreak::StringToTiebreaker(eventSettingsData.secondTiebreaker);
    *m_ThirdTiebreaker = Tiebreak::StringToTiebreaker(eventSettingsData.thirdTiebreaker);
    *m_FourthTiebreaker = Tiebreak::StringToTiebreaker(eventSettingsData.fourthTiebreaker);
}

void MainWindow::ResetPlayerTable()
{
    UtilDebug("Reset Player Table");

    int i = m_Ui->TournamentCreatorPlayerTableWidget->rowCount();
    for(int j = 0; j < i; ++j)
    {
        m_Ui->TournamentCreatorPlayerTableWidget->removeRow(0);
    }

    m_Ui->TournamentCreatorPlayerTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Player Name"));
    m_Ui->TournamentCreatorPlayerTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Player Seed"));
}

void MainWindow::UpdatePlayerTable()
{
    UtilDebug("Updating Player Table");
    ResetPlayerTable();

    for(const auto &player : *m_TempPlayerList)
    {
        if(player.GetName() != "")
        {
            int i = m_Ui->TournamentCreatorPlayerTableWidget->rowCount();
            m_Ui->TournamentCreatorPlayerTableWidget->insertRow(i);
            m_Ui->TournamentCreatorPlayerTableWidget->setItem(i, 0, new QTableWidgetItem(player.GetName().c_str()));
            m_Ui->TournamentCreatorPlayerTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(player.GetSeed()))));
        }
    }
}
