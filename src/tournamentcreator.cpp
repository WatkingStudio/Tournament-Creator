#include "include/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::loadTournamentCreatorPage()
{
    ui->stackedWidget->setCurrentIndex(Pages::TOURNAMENT_CREATOR);
    ui->pageTitleWidget->setText("Tournament Creator");
    ui->eventSettingsTPValues->setEnabled(false);
    ui->eventSettingsTPValues->setVisible(false);
    ui->eventSettingsNumberRoundsWidget->setEnabled(false);
    ui->eventSettingsNumberRoundsWidget->setVisible(false);
    ui->usingSeededPlayersWidget->setEnabled(false);
    ui->usingSeededPlayersWidget->setVisible(false);

    m_TournamentCreatorSelectedCol = -1;
    m_TournamentCreatorSelectedRow = -1;

    resetPlayerTable();
}

void MainWindow::updatePlayerTable()
{
    utilDebug("Updating Player Table");
    resetPlayerTable();

    for(auto it = m_TempPlayerList.begin(); it != m_TempPlayerList.end(); ++it)
    {
        if(it->getName() != "")
        {
            int i = ui->tournamentCreatorPlayerTableWidget->rowCount();
            ui->tournamentCreatorPlayerTableWidget->insertRow(i);
            ui->tournamentCreatorPlayerTableWidget->setItem(i, 0, new QTableWidgetItem(it->getName().c_str()));
            ui->tournamentCreatorPlayerTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(it->getSeed()))));
        }
    }
}

void MainWindow::resetPlayerTable()
{
    utilDebug("Reset Player Table");

    int i = ui->tournamentCreatorPlayerTableWidget->rowCount();
    for(int j = 0; j < i; ++j)
    {
        ui->tournamentCreatorPlayerTableWidget->removeRow(0);
    }

    ui->tournamentCreatorPlayerTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Player Name"));
    ui->tournamentCreatorPlayerTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Player Seed"));
}

void MainWindow::on_removePlayerButton_clicked()
{
    utilDebug("Remove Player Clicked");
    if(m_TournamentCreatorSelectedRow != -1)
    {
        ui->tournamentCreatorPlayerTableWidget->removeRow(m_TournamentCreatorSelectedRow);
        m_TempPlayerList.erase(m_TempPlayerList.begin() + m_TournamentCreatorSelectedRow);
        m_TournamentCreatorSelectedRow = -1;
    }
}

void MainWindow::on_addPlayerButton_clicked()
{
    if(ui->playerNameInputLineEdit->displayText() != "")
    {
        utilDebug("Add Player");

        Player newPlayer;
        newPlayer.setName(ui->playerNameInputLineEdit->text().toStdString());
        newPlayer.setSeed(ui->playerSeedInputLineEdit->text().toInt());

        m_TempPlayerList.push_back(newPlayer);

        updatePlayerTable();
        ui->playerNameInputLineEdit->clear();
        ui->playerSeedInputLineEdit->clear();
    }
}

void MainWindow::on_saveEventTournamentCreatorButton_clicked()
{
    utilDebug("Tournament Creator Save Event Button Clicked");
    saveEventData();
}

void MainWindow::on_eventSettingsButton_clicked()
{
    ui->eventSettingsTPValues->setEnabled(!ui->eventSettingsTPValues->isEnabled());
    ui->eventSettingsTPValues->setVisible(!ui->eventSettingsTPValues->isVisible());
    ui->eventSettingsNumberRoundsWidget->setEnabled(!ui->eventSettingsNumberRoundsWidget->isEnabled());
    ui->eventSettingsNumberRoundsWidget->setVisible(!ui->eventSettingsNumberRoundsWidget->isVisible());
    ui->usingSeededPlayersWidget->setEnabled(!ui->usingSeededPlayersWidget->isEnabled());
    ui->usingSeededPlayersWidget->setVisible(!ui->usingSeededPlayersWidget->isVisible());
    m_EventSettingsActive = ui->eventSettingsTPValues->isEnabled();
}

void MainWindow::on_continueTournamentCreatorButton_clicked()
{
    utilDebug("Tournament Creator Continue Button Clicked");
    if(m_TempPlayerList.size() > 0)
    {
        m_MainPlayerList.clear();
        for(auto it = m_TempPlayerList.begin(); it != m_TempPlayerList.end(); ++it)
        {
            m_MainPlayerList.push_back(*it);
        }

        loadMatchupsPage();
    }
    else
    {
        QMessageBox::warning(this, "Warning", "No Players Detected.");
    }

    if(m_EventSettingsActive)
    {
        m_NumberOfRounds = ui->eventSettingsNumberRoundsLineEdit->displayText().toInt();
        m_WinValue = ui->eventSettingsWinLineEdit->displayText().toInt();
        m_DrawValue = ui->eventSettingsDrawLineEdit->displayText().toInt();
        m_LossValue = ui->eventSettingsLossLineEdit->displayText().toInt();
        m_MostSportingValue = ui->eventSettingsMostSportingLineEdit->displayText().toInt();
        m_BestPaintedValue = ui->eventSettingsBestPaintedLineEdit->displayText().toInt();
    }
    saveEventData();
}

void MainWindow::on_backTournamentCreatorButton_clicked()
{
    loadStartPage();
}

void MainWindow::on_usingSeededPlayersCheckBox_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        m_UsingSeed = true;
    }
    else
    {
        m_UsingSeed = false;
    }
}
