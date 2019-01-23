#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "time.h"


void MainWindow::on_matchupsBackButton_clicked()
{
    utilDebug("Matchups Back Button Selected");
    loadTournamentCreatorPage();
}

void MainWindow::on_matchupsEnterResultsButton_clicked()
{
    utilDebug("Enter Results Button Selected");
    if(resultsValid())
    {
        utilDebug("Results are valid");
        if(ui->matchupsPlayerTableWidget->item(m_MatchupsSelectedRow, 0)->text() == ui->matchupsPlayerOneNameLabel->text() && ui->matchupsPlayerTableWidget->item(m_MatchupsSelectedRow, 3)->text() == ui->matchupsPlayerTwoNameLabel->text())
        {
            ui->matchupsPlayerTableWidget->setItem(m_MatchupsSelectedRow, 1, new QTableWidgetItem(ui->matchupsPlayerOneComboBox->currentText()));
            ui->matchupsPlayerTableWidget->setItem(m_MatchupsSelectedRow, 2, new QTableWidgetItem(ui->matchupsPlayerOneSpinBox->text()));
            ui->matchupsPlayerTableWidget->setItem(m_MatchupsSelectedRow, 4, new QTableWidgetItem(ui->matchupsPlayerTwoComboBox->currentText()));
            ui->matchupsPlayerTableWidget->setItem(m_MatchupsSelectedRow, 5, new QTableWidgetItem(ui->matchupsPlayerTwoSpinBox->text()));
        }
        else
        {
            utilLog("Error: Matchup doesnt match");
            utilLog(ui->matchupsPlayerTableWidget->item(m_MatchupsSelectedRow, 0)->text().toStdString());
            utilLog(ui->matchupsPlayerOneNameLabel->text().toStdString());
            utilLog(ui->matchupsPlayerTableWidget->item(m_MatchupsSelectedRow, 3)->text().toStdString());
            utilLog(ui->matchupsPlayerTwoNameLabel->text().toStdString());
        }

        resetRoundInput();
        ui->matchupsModifyMatchupsButton->setEnabled(false);
    }
    else
    {
        utilDebug("Results aren't valid");
        QMessageBox::warning(this, "Invalid Results", "Input results are not valid. Please make sure you have input them correctly.");
    }
}

void MainWindow::on_matchupsModifyMatchupsButton_clicked()
{
    utilDebug("Modify Matchups");

    if(ui->matchupsPlayerTableWidget->rowCount() > m_MatchupsSelectedRow + 1)
    {
        utilDebug(std::to_string(ui->matchupsPlayerTableWidget->rowCount()));
        utilDebug(std::to_string(m_MatchupsSelectedRow));
        //Get a temporary copy of both matchups
        std::pair<Player, Player> tempMatchupOne;
        tempMatchupOne = m_CurrentRoundMatchups.at(m_MatchupsSelectedRow);
        std::pair<Player, Player> tempMatchupTwo;
        tempMatchupTwo = m_CurrentRoundMatchups.at(m_MatchupsSelectedRow + 1);

        //Get a temporary copy of the two players to swap
        Player tempPlayerOne = tempMatchupOne.second;
        Player tempPlayerTwo = tempMatchupTwo.first;

        //Re-assign the two players
        tempMatchupOne.second = tempPlayerTwo;
        tempMatchupTwo.first = tempPlayerOne;

        //Update both matchups
        m_CurrentRoundMatchups.at(m_MatchupsSelectedRow).operator =(tempMatchupOne);
        m_CurrentRoundMatchups.at(m_MatchupsSelectedRow + 1).operator =(tempMatchupTwo);

        updateMatchupsTable();
        resetRoundInput();
        ui->matchupsModifyMatchupsButton->setEnabled(false);
    }
    else
    {
        QMessageBox::warning(this, "Invalid Action", "Selected Matchup cannot be modified. Try using Advanced Matchup Modifiction (coming soon).");
    }
}

void MainWindow::on_matchupsNextRoundButton_clicked()
{
    utilDebug("Next Round Button Clicked");

    if(isRoundFinished())
    {
        updateScores();
        updateRankings();
        createRankedMatchup();

        utilDebug("Round " + std::to_string(m_CurrentRoundNumber) + " Finished");

        m_CurrentRoundNumber++;
        m_ActiveRoundNumber = m_CurrentRoundNumber;

        ui->matchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));
        utilDebug("Round " + std::to_string(m_CurrentRoundNumber) + " Started");
    }
    else
    {
        QMessageBox::warning(this, "Unfinished Round", "Round has not finished. Please make sure that all matchup data has been set.");
    }

    saveEventData();

    if(m_CurrentRoundNumber > m_NumberOfRounds)
    {
        utilDebug("Finished Final Round");
        loadResultsPage();
    }
}

void MainWindow::loadMatchupsPage()
{
    utilDebug("Load Matchups Page");
    ui->stackedWidget->setCurrentIndex(Pages::MATCHUPS_PAGE);
    ui->matchupsPreviousRoundButton->setEnabled(false);
    ui->matchupsCurrentRoundButton->setEnabled(false);
    ui->matchupsModifyMatchupsButton->setEnabled(false);
    ui->matchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));
    m_CurrentRoundNumber = 1;
    m_ActiveRoundNumber = 1;
    m_MatchupsSelectedCol = -1;
    m_MatchupsSelectedRow = -1;

    resetRoundInput();

    //Create Initial Matchups
    m_AllRoundMatchups.clear();
    m_CurrentRoundMatchups.clear();
    setInitialMatchups();

    //List Players
    for(auto it = m_MainPlayerList.begin(); it != m_MainPlayerList.end(); ++it)
    {
        ui->matchupsPlayerListWidget->addItem(QString::fromStdString(it->getName()));
    }
}

void MainWindow::loadMatchupsPageFromLoadedEvent()
{
    utilLog("Load Matchups Page from Loaded Event");
    ui->stackedWidget->setCurrentIndex(Pages::MATCHUPS_PAGE);
    ui->matchupsPreviousRoundButton->setEnabled(false);
    ui->matchupsCurrentRoundButton->setEnabled(false);
    ui->matchupsModifyMatchupsButton->setEnabled(false);
    ui->matchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));

    resetRoundInput();

    setLoadedEventMatchups();

    //List Players
    ui->matchupsPlayerListWidget->clear();
    for(auto it = m_MainPlayerList.begin(); it != m_MainPlayerList.end(); ++it)
    {
        ui->matchupsPlayerListWidget->addItem(QString::fromStdString(it->getName()));
    }
}

void MainWindow::setLoadedEventMatchups()
{
    m_CurrentRoundMatchups = m_AllRoundMatchups.at(m_CurrentRoundNumber - 1);

    updateMatchupsTable();
}

void MainWindow::setInitialMatchups()
{
    utilDebug("Set Initial Matchups");
    if(m_MainPlayerList.size() % 2 != 0)
    {
        utilLog("Added Ringer");
        Player ringer;
        ringer.setName("Ringer");
        ringer.setSeed(0);
        m_MainPlayerList.push_back(ringer);
    }

    if(m_UsingSeed)
    {
        createSeededMatchup();
    }
    else
    {
        createRandomMatchup();
    }
}

void MainWindow::createSeededMatchup()
{
    utilLog("Functionality Not Implemented: createSeededMatchup");
}

void MainWindow::createRandomMatchup()
{
    utilDebug("Create Random Matchups");
    int playerNumber = m_MainPlayerList.size();

    for(auto it : m_MainPlayerList)
    {
        it.setFirstRoundSet(false);
    }

    for(int i = 1; i < playerNumber/2 + 1; ++i)
    {
        Player one = findRandomPlayer(playerNumber);
        Player two = findRandomPlayer(playerNumber);

        m_CurrentRoundMatchups.push_back(std::pair<Player,Player>(one, two));
    }
    m_AllRoundMatchups.push_back(m_CurrentRoundMatchups);
    updateMatchupsTable();
}

void MainWindow::updateScores()
{
    int rowCount = ui->matchupsPlayerTableWidget->rowCount();
    
    //For each row in the table
    for(int i = 0; i < rowCount; ++i)
    {
        //Player one and player two
        std::string playerOneName = ui->matchupsPlayerTableWidget->item(i, 0)->text().toStdString();
        std::string playerTwoName = ui->matchupsPlayerTableWidget->item(i, 3)->text().toStdString();
        
        int playerOneScore = ui->matchupsPlayerTableWidget->item(i, 2)->text().toInt();
        int playerTwoScore = ui->matchupsPlayerTableWidget->item(i, 5)->text().toInt();
        std::string playerOneResult = ui->matchupsPlayerTableWidget->item(i, 1)->text().toStdString();
        std::string playerTwoResult = ui->matchupsPlayerTableWidget->item(i, 4)->text().toStdString();
        
        //Find them in the player list
        for(int i = 0; i < m_MainPlayerList.size(); ++i)
        {
            //Update the players values.
            if(m_MainPlayerList.at(i).getName() == playerOneName)
            {
                m_MainPlayerList.at(i).addVPs(playerOneScore);
                m_MainPlayerList.at(i).addVPDiff(playerOneScore - playerTwoScore);
                m_MainPlayerList.at(i).addResult(playerOneResult);
            }
            else if(m_MainPlayerList.at(i).getName() == playerTwoName)
            {
                m_MainPlayerList.at(i).addVPs(playerTwoScore);
                m_MainPlayerList.at(i).addVPDiff(playerTwoScore - playerOneScore);
                m_MainPlayerList.at(i).addResult(playerTwoResult);
            }
        }
    }
}

void MainWindow::createRankedMatchup()
{
    utilDebug("Create Ranked Matchup");
    m_CurrentRoundMatchups.clear();
    for(int i = 0; i < m_MainPlayerList.size(); i += 2)
    {
        Player one = m_MainPlayerList.at(i);
        Player two = m_MainPlayerList.at(i + 1);

        m_CurrentRoundMatchups.push_back(std::pair<Player, Player>(one, two));
    }
    m_AllRoundMatchups.push_back(m_CurrentRoundMatchups);
    updateMatchupsTable();

    utilLog("Funcationality Not implemented: createRankedMatchup");
}

void MainWindow::updateRankings()
{
    std::vector<Player> tempList = m_MainPlayerList;
    std::vector<Player> tempRankings;
    Player highestRank;

    for(int i = 0; i < m_MainPlayerList.size(); ++i)
    {
        highestRank = tempList.at(0);

        for(auto it = tempList.begin() + 1; it != tempList.end(); ++it)
        {
            if(it->getTPs() > highestRank.getTPs())
            {
                highestRank = *it;
            }
            else if(it->getTPs() == highestRank.getTPs())
            {
                //First tie-breaker
                if(it->getVPs() > highestRank.getVPs())
                {
                    highestRank = *it;
                }
                else if(it->getVPs() == highestRank.getVPs())
                {
                    //Second tie-breaker
                    if(it->getVPDiff() > highestRank.getVPDiff())
                    {
                        highestRank = *it;
                    }
                }
            }
        }
        for(auto it = tempList.begin(); it != tempList.end(); ++it)
        {
            if(it->getName() == highestRank.getName())
            {
                tempRankings.push_back(highestRank);
                tempList.erase(it);
                break;
            }
        }
    }
    
    m_MainPlayerList = tempRankings;
}

void MainWindow::resetMatchupsTable()
{
    utilDebug("Reset Matchups Table");
    ui->matchupsPlayerTableWidget->clear();

    int i = ui->matchupsPlayerTableWidget->rowCount();
    for(int j = 0; j < i; ++j)
    {
        ui->matchupsPlayerTableWidget->removeRow(0);
    }

    ui->matchupsPlayerTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Player One Name"));
    ui->matchupsPlayerTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Player One Result"));
    ui->matchupsPlayerTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Player One VPs"));
    ui->matchupsPlayerTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Player Two Name"));
    ui->matchupsPlayerTableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Player Two Result"));
    ui->matchupsPlayerTableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Player Two VPs"));
}

void MainWindow::updateMatchupsTable()
{
    utilDebug("Update Matchups Table");
    resetMatchupsTable();

    for(int i = 0; i < m_CurrentRoundMatchups.size(); ++i)
    {
        ui->matchupsPlayerTableWidget->insertRow(ui->matchupsPlayerTableWidget->rowCount());
        ui->matchupsPlayerTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(m_CurrentRoundMatchups.at(i).first.getName())));
        ui->matchupsPlayerTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString("")));
        ui->matchupsPlayerTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString("")));
        ui->matchupsPlayerTableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(m_CurrentRoundMatchups.at(i).second.getName())));
        ui->matchupsPlayerTableWidget->setItem(i, 4, new QTableWidgetItem(QString::fromStdString("")));
        ui->matchupsPlayerTableWidget->setItem(i, 5, new QTableWidgetItem(QString::fromStdString("")));
    }
}

void MainWindow::resetRoundInput()
{
    utilDebug("Reset Round Input");
    ui->matchupsPlayerOneNameLabel->setText(QString("Player One"));
    ui->matchupsPlayerOneComboBox->setCurrentIndex(0);
    ui->matchupsPlayerOneSpinBox->setValue(0);
    ui->matchupsPlayerTwoNameLabel->setText(QString("Player Two"));
    ui->matchupsPlayerTwoComboBox->setCurrentIndex(0);
    ui->matchupsPlayerTwoSpinBox->setValue(0);

    disableRoundInput();
}

void MainWindow::disableRoundInput()
{
    utilDebug("Disable Round Input");
    ui->matchupsPlayerOneComboBox->setEnabled(false);
    ui->matchupsPlayerOneNameLabel->setEnabled(false);
    ui->matchupsPlayerOneSpinBox->setEnabled(false);
    ui->matchupsPlayerTwoComboBox->setEnabled(false);
    ui->matchupsPlayerTwoNameLabel->setEnabled(false);
    ui->matchupsPlayerTwoSpinBox->setEnabled(false);
    ui->matchupsEnterResultsButton->setEnabled(false);
}

void MainWindow::enableRoundInput()
{
    utilDebug("Enable Round Input");
    ui->matchupsPlayerOneComboBox->setEnabled(true);
    ui->matchupsPlayerOneNameLabel->setEnabled(true);
    ui->matchupsPlayerOneSpinBox->setEnabled(true);
    ui->matchupsPlayerTwoComboBox->setEnabled(true);
    ui->matchupsPlayerTwoNameLabel->setEnabled(true);
    ui->matchupsPlayerTwoSpinBox->setEnabled(true);
    ui->matchupsEnterResultsButton->setEnabled(true);
}

void MainWindow::updateRoundInput()
{
    utilDebug("Update Round Input");
    enableRoundInput();

    std::pair<Player, Player> tempMatchup;
    tempMatchup = m_CurrentRoundMatchups.at(m_MatchupsSelectedRow);

    ui->matchupsPlayerOneNameLabel->setText(QString::fromStdString(tempMatchup.first.getName()));
    ui->matchupsPlayerTwoNameLabel->setText(QString::fromStdString(tempMatchup.second.getName()));
}

bool MainWindow::isRoundFinished() const
{
    for(int i = 0; i < ui->matchupsPlayerTableWidget->rowCount(); ++i)
    {
        if(ui->matchupsPlayerTableWidget->item(i, 1)->text() == QString::fromStdString("") || ui->matchupsPlayerTableWidget->item(i, 4)->text() == QString::fromStdString(""))
        {
            return false;
        }
    }

    return true;
}

bool MainWindow::resultsValid() const
{
    if(ui->matchupsPlayerOneComboBox->currentText() == QString("Win") && ui->matchupsPlayerTwoComboBox->currentText() == QString("Loss"))
    {
        if(!m_TableIsVictory)
        {
            if(ui->matchupsPlayerOneSpinBox->value() <= ui->matchupsPlayerTwoSpinBox->value())
            {
                return false;
            }
        }
        return true;
    }
    else if(ui->matchupsPlayerOneComboBox->currentText() == QString("Draw") && ui->matchupsPlayerTwoComboBox->currentText() == QString("Draw"))
    {
        if(ui->matchupsPlayerOneSpinBox->value() != ui->matchupsPlayerTwoSpinBox->value())
        {
            return false;
        }
        return true;
    }
    else if(ui->matchupsPlayerOneComboBox->currentText() == QString("Loss") && ui->matchupsPlayerTwoComboBox->currentText() == QString("Win"))
    {
        if(!m_TableIsVictory)
        {
            if(ui->matchupsPlayerOneSpinBox->value() >= ui->matchupsPlayerTwoSpinBox->value())
            {
                return false;
            }
        }
        return true;
    }

    return false;
}

Player MainWindow::findRandomPlayer(int playerCount)
{
    srand(time(NULL));

    bool searching = true;
    int number;
    while(searching)
    {
        number = rand() % playerCount;
        if(!m_MainPlayerList.at(number).isFirstRoundSet())
        {
            m_MainPlayerList.at(number).setFirstRoundSet(true);
            searching = false;
        }
    }
    return m_MainPlayerList.at(number);
}
