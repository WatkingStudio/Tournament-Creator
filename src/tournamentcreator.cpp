#include "include/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::loadTournamentCreatorPage()
{
    ui->stackedWidget->setCurrentIndex(Pages::TOURNAMENT_CREATOR);
    ui->pageTitleWidget->setText("Tournament Creator");

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
    m_EventSettingsWidget.show();
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

    if(m_EventSettingsWidget.isVisible())
        m_EventSettingsWidget.hide();

    saveEventData();
}

void MainWindow::on_backTournamentCreatorButton_clicked()
{
    loadStartPage();
}

void MainWindow::receiveEventSettings(int winTPs, int drawTPs, int lossTPs, int mostSportingTPs, int bestPaintedTPs, bool usingSeeded, int numberOfRounds, std::string firstTiebreaker, std::string secondTiebreaker, std::string thirdTiebreaker, std::string fourthTiebreaker)
{
    utilDebug("Event Settings Changed");

    if(winTPs > 0)
        m_WinValue = winTPs;
    if(drawTPs > 0)
        m_DrawValue = drawTPs;
    if(lossTPs > 0)
        m_LossValue = lossTPs;
    if(mostSportingTPs >= 0)
        m_MostSportingValue = mostSportingTPs;
    if(bestPaintedTPs >= 0)
        m_BestPaintedValue = bestPaintedTPs;
    m_UsingSeed = usingSeeded;
    if(numberOfRounds > 0)
        m_NumberOfRounds = numberOfRounds;
    if(firstTiebreaker != *Tiebreak::NoneString)
        m_FirstTiebreaker = Tiebreak::StringToTiebreaker(firstTiebreaker);
    if(secondTiebreaker != *Tiebreak::NoneString)
        m_SecondTiebreaker = Tiebreak::StringToTiebreaker(secondTiebreaker);
    if(thirdTiebreaker != *Tiebreak::NoneString)
        m_ThirdTiebreaker = Tiebreak::StringToTiebreaker(thirdTiebreaker);
    if(fourthTiebreaker != *Tiebreak::NoneString)
        m_FourthTiebreaker = Tiebreak::StringToTiebreaker(fourthTiebreaker);
}
