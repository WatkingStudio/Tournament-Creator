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
            m_EventDataFileName = filePath.toStdString();
            break;
        }
    }
    CreateFile(m_EventDataFileName.c_str(), *m_EventDirectory);

    ui->stackedWidget->setCurrentIndex(Pages::TOURNAMENT_CREATOR);
    ui->pageTitleWidget->setText("Tournament Creator");

    m_TournamentCreatorSelectedCol = -1;
    m_TournamentCreatorSelectedRow = -1;

    ResetPlayerTable();
}

void MainWindow::UpdatePlayerTable()
{
    UtilDebug("Updating Player Table");
    ResetPlayerTable();

    for(const auto &player : *m_TempPlayerList)
    {
        if(player.getName() != "")
        {
            int i = ui->tournamentCreatorPlayerTableWidget->rowCount();
            ui->tournamentCreatorPlayerTableWidget->insertRow(i);
            ui->tournamentCreatorPlayerTableWidget->setItem(i, 0, new QTableWidgetItem(player.getName().c_str()));
            ui->tournamentCreatorPlayerTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(player.getSeed()))));
        }
    }
}

void MainWindow::ResetPlayerTable()
{
    UtilDebug("Reset Player Table");

    int i = ui->tournamentCreatorPlayerTableWidget->rowCount();
    for(int j = 0; j < i; ++j)
    {
        ui->tournamentCreatorPlayerTableWidget->removeRow(0);
    }

    ui->tournamentCreatorPlayerTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Player Name"));
    ui->tournamentCreatorPlayerTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Player Seed"));
}

void MainWindow::on_RemovePlayerButton_clicked()
{
    UtilDebug("Remove Player Clicked");
    if(m_TournamentCreatorSelectedRow != -1)
    {
        ui->tournamentCreatorPlayerTableWidget->removeRow(m_TournamentCreatorSelectedRow);
        m_TempPlayerList->erase(m_TempPlayerList->begin() + m_TournamentCreatorSelectedRow);
        m_TournamentCreatorSelectedRow = -1;
    }
}

void MainWindow::on_AddPlayerButton_clicked()
{
    UtilDebug("Add Player Clicked");
    if(ui->playerNameInputLineEdit->displayText() != "")
    {
        UtilDebug("Add Player");

        Player newPlayer;
        newPlayer.setName(ui->playerNameInputLineEdit->text().toStdString());
        newPlayer.setSeed(ui->playerSeedInputLineEdit->text().toInt());

        m_TempPlayerList->push_back(newPlayer);

        UpdatePlayerTable();
        ui->playerNameInputLineEdit->clear();
        ui->playerSeedInputLineEdit->clear();
    }
}

void MainWindow::on_SaveEventTournamentCreatorButton_clicked()
{
    UtilDebug("Tournament Creator Save Event Button Clicked");
    saveEventData();
}

void MainWindow::on_EventSettingsButton_clicked()
{
    UtilDebug("Event Settings Clicked");
    m_EventSettingsWidget.show();
}

void MainWindow::on_ContinueTournamentCreatorButton_clicked()
{
    UtilDebug("Tournament Creator Continue Button Clicked");
    if(m_TempPlayerList->size() > 0)
    {
        m_MainPlayerList.clear();
        for(const auto &player : *m_TempPlayerList)
        {
            m_MainPlayerList.push_back(player);
        }

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

    saveEventData();
}

void MainWindow::on_BackTournamentCreatorButton_clicked()
{
    loadStartPage();
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
        m_MostSportingValue = eventSettingsData.mostSportingTPs;
    }
    if(eventSettingsData.bestPaintedTPs >= 0)
    {
        m_BestPaintedValue = eventSettingsData.bestPaintedTPs;
    }
    m_UsingSeed = eventSettingsData.usingSeeded;
    if(eventSettingsData.numberOfRounds > 0)
    {
        m_NumberOfRounds = eventSettingsData.numberOfRounds;
    }

    m_FirstTiebreaker = Tiebreak::StringToTiebreaker(eventSettingsData.firstTiebreaker);
    m_SecondTiebreaker = Tiebreak::StringToTiebreaker(eventSettingsData.secondTiebreaker);
    m_ThirdTiebreaker = Tiebreak::StringToTiebreaker(eventSettingsData.thirdTiebreaker);
    m_FourthTiebreaker = Tiebreak::StringToTiebreaker(eventSettingsData.fourthTiebreaker);
}
