#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "time.h"
#include <set>

// This .cpp file is used to handle the Matchups Page

void MainWindow::on_MatchupsBackButton_clicked()
{
    UtilDebug("Matchups Back Button Selected");
    LoadTournamentCreatorPage();
}

void MainWindow::on_MatchupsEnterResultsButton_clicked()
{
    UtilDebug("Enter Results Button Selected");
    if(ResultsValid())
    {
        UtilLog("Results are valid");
        if(ui->MatchupsPlayerTableWidget->item(*m_MatchupsSelectedRow, 0)->text() == ui->MatchupsPlayerOneNameLabel->text()
                && ui->MatchupsPlayerTableWidget->item(*m_MatchupsSelectedRow, 3)->text() == ui->MatchupsPlayerTwoNameLabel->text())
        {
            ui->MatchupsPlayerTableWidget->setItem(*m_MatchupsSelectedRow, 1, new QTableWidgetItem(ui->MatchupsPlayerOneComboBox->currentText()));
            ui->MatchupsPlayerTableWidget->setItem(*m_MatchupsSelectedRow, 2, new QTableWidgetItem(ui->MatchupsPlayerOneSpinBox->text()));
            ui->MatchupsPlayerTableWidget->setItem(*m_MatchupsSelectedRow, 4, new QTableWidgetItem(ui->MatchupsPlayerTwoComboBox->currentText()));
            ui->MatchupsPlayerTableWidget->setItem(*m_MatchupsSelectedRow, 5, new QTableWidgetItem(ui->MatchupsPlayerTwoSpinBox->text()));
        }
        else
        {
            UtilLog("Error: Matchup doesnt match");
            UtilLog(ui->MatchupsPlayerTableWidget->item(*m_MatchupsSelectedRow, 0)->text().toStdString());
            UtilLog(ui->MatchupsPlayerOneNameLabel->text().toStdString());
            UtilLog(ui->MatchupsPlayerTableWidget->item(*m_MatchupsSelectedRow, 3)->text().toStdString());
            UtilLog(ui->MatchupsPlayerTwoNameLabel->text().toStdString());
        }

        ResetRoundInput();
        ui->MatchupsModifyMatchupsButton->setEnabled(false);
    }
    else
    {
        UtilLog("Results aren't valid");
        QMessageBox::warning(this, "Invalid Results", "Input results are not valid. Please make sure you have input them correctly.");
    }
}

void MainWindow::on_MatchupsModifyMatchupsButton_clicked()
{
    UtilDebug("Modify Matchups");

    if(ui->MatchupsPlayerTableWidget->rowCount() > *m_MatchupsSelectedRow + 1)
    {
        UtilLog(std::to_string(ui->MatchupsPlayerTableWidget->rowCount()));
        UtilLog(std::to_string(*m_MatchupsSelectedRow));

        Player playerOne = m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow).second;
        Player playerTwo = m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow + 1).first;

        m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow).second.operator=(playerTwo);
        m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow + 1).first.operator=(playerOne);

        UpdateMatchupsTable();
        ResetRoundInput();
        ui->MatchupsModifyMatchupsButton->setEnabled(false);
    }
    else
    {
        QMessageBox::warning(this, "Invalid Action", "Selected Matchup cannot be modified. Try using Advanced Matchup Modifiction (coming soon).");
    }
}

void MainWindow::on_MatchupsNextRoundButton_clicked()
{
    UtilDebug("Next Round Button Clicked");

    if(IsRoundFinished())
    {
        UpdateScores();
        UpdateRankings();
        CreateRankedMatchup();

        UtilLog("Round " + std::to_string(m_CurrentRoundNumber) + " Finished");

        m_ActiveRoundNumber = ++m_CurrentRoundNumber;

        ui->MatchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));
        UtilLog("Round " + std::to_string(m_CurrentRoundNumber) + " Started");
    }
    else
    {
        QMessageBox::warning(this, "Unfinished Round", "Round has not finished. Please make sure that all matchup data has been set.");
    }

    saveEventData();

    if(m_CurrentRoundNumber > m_NumberOfRounds)
    {
        UtilLog("Finished Final Round");
        loadResultsPage();
    }
}

void MainWindow::on_MatchupsResetMatchupTable_clicked()
{
    UtilDebug("Reset Matchups Clicked");
    *m_CurrentRoundMatchups = m_AllRoundMatchups->at(m_CurrentRoundNumber - 1);
    UpdateMatchupsTable();
}

void MainWindow::on_MatchupsDirectMatchupSwapButton_clicked()
{
    UtilDebug("Direct Matchup Clicked");
    m_MatchUpSwapWidget.show();
    m_MatchUpSwapWidget.SetUp(m_MainPlayerList, *m_CurrentRoundMatchups);
}

void MainWindow::NewMatchUpsFromSwap(const Player &playerOne, const Player &playerTwo, const Player &playerThree, const Player &playerFour)
{
    UpdateTableMatchUp(playerOne, playerTwo);
    UpdateTableMatchUp(playerThree, playerFour);
    UpdateMatchupsTable();
}

void MainWindow::LoadMatchupsPage()
{
    UtilDebug("Load Matchups Page");
    ui->stackedWidget->setCurrentIndex(Pages::MATCHUPS_PAGE);
    ui->MatchupsPreviousRoundButton->setEnabled(false);
    ui->MatchupsCurrentRoundButton->setEnabled(false);
    ui->MatchupsModifyMatchupsButton->setEnabled(false);
    m_CurrentRoundNumber = 1;
    ui->MatchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));
    m_ActiveRoundNumber = 1;
    *m_MatchupsSelectedCol = -1;
    *m_MatchupsSelectedRow = -1;

    ResetRoundInput();

    //Create Initial Matchups
    m_AllRoundMatchups->clear();
    m_CurrentRoundMatchups->clear();
    CreateInitialMatchups();

    UpdatePlayerRankingList();
}

void MainWindow::UpdatePlayerRankingList()
{
    UtilDebug("Update Player Rankings List");
    ui->MatchupsPlayerListWidget->clear();

    for(const auto &player : m_MainPlayerList)
    {
        ui->MatchupsPlayerListWidget->addItem(QString::fromStdString(player.getName()));
    }
}

void MainWindow::LoadMatchupsPageFromLoadedEvent()
{
    UtilDebug("Load Matchups Page from Loaded Event");
    ui->stackedWidget->setCurrentIndex(Pages::MATCHUPS_PAGE);
    ui->MatchupsPreviousRoundButton->setEnabled(false);
    ui->MatchupsCurrentRoundButton->setEnabled(false);
    ui->MatchupsModifyMatchupsButton->setEnabled(false);
    ui->MatchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));

    ResetRoundInput();

    UtilDebug("Event Matchups Loaded");
    *m_CurrentRoundMatchups = m_AllRoundMatchups->at(m_CurrentRoundNumber - 1);

    UpdateMatchupsTable();

    //List Players
    ui->MatchupsPlayerListWidget->clear();

    for(const auto &player : m_MainPlayerList)
    {
        ui->MatchupsPlayerListWidget->addItem(QString::fromStdString(player.getName()));
    }
}

void MainWindow::CreateInitialMatchups()
{
    UtilDebug("Set Initial Matchups");
    if(m_MainPlayerList.size() % 2 != 0)
    {
        UtilLog("Added Ringer");
        Player ringer;
        ringer.setName("Ringer");
        ringer.setSeed(0);
        m_MainPlayerList.push_back(ringer);
    }

    if(m_UsingSeed)
    {
        CreateSeededMatchup();
    }
    else
    {
        CreateRandomMatchup();
    }
}

void MainWindow::CreateSeededMatchup()
{
    UtilDebug("Create Seeded Matchups");

    auto playerSeedComparison = [](Player one, Player two){
        return one.getSeed() < two.getSeed() ? true : false;
    };
    std::set<Player, decltype(playerSeedComparison)> temporarySet(playerSeedComparison);
    for(auto &player : m_MainPlayerList)
    {
        player.setFirstRoundSet(false);
        temporarySet.insert(player);
    }

    m_MainPlayerList.clear();

    for(const auto &player : temporarySet)
    {
        m_MainPlayerList.push_back(player);
    }

    for(uint i = 0; i < m_MainPlayerList.size()/2; ++i)
    {
        Player one = m_MainPlayerList.at(0 + i);
        Player two = m_MainPlayerList.at(m_MainPlayerList.size() - 1 - i);

        m_CurrentRoundMatchups->push_back(std::pair<Player, Player>(one, two));
    }

    m_AllRoundMatchups->push_back(*m_CurrentRoundMatchups);
    UpdateMatchupsTable();
}

void MainWindow::CreateRandomMatchup()
{
    UtilDebug("Create Random Matchups");
    int playerNumber = m_MainPlayerList.size();

    for(auto it : m_MainPlayerList)
    {
        it.setFirstRoundSet(false);
    }

    for(int i = 1; i < playerNumber/2 + 1; ++i)
    {
        Player one = FindRandomPlayer(playerNumber);
        Player two = FindRandomPlayer(playerNumber);

        m_CurrentRoundMatchups->push_back(std::pair<Player,Player>(one, two));
    }
    m_AllRoundMatchups->push_back(*m_CurrentRoundMatchups);
    UpdateMatchupsTable();
}

void MainWindow::UpdateScores()
{
    UtilDebug("Update Scores");
    int rowCount = ui->MatchupsPlayerTableWidget->rowCount();
    
    //For each row in the table
    for(int i = 0; i < rowCount; ++i)
    {
        //Player one and player two
        std::string playerOneName = ui->MatchupsPlayerTableWidget->item(i, 0)->text().toStdString();
        std::string playerTwoName = ui->MatchupsPlayerTableWidget->item(i, 3)->text().toStdString();
        
        int playerOneScore = ui->MatchupsPlayerTableWidget->item(i, 2)->text().toInt();
        int playerTwoScore = ui->MatchupsPlayerTableWidget->item(i, 5)->text().toInt();
        std::string playerOneResult = ui->MatchupsPlayerTableWidget->item(i, 1)->text().toStdString();
        std::string playerTwoResult = ui->MatchupsPlayerTableWidget->item(i, 4)->text().toStdString();
        
        //Find them in the player list
        for(uint i = 0; i < m_MainPlayerList.size(); ++i)
        {
            //Update the players values.
            if(m_MainPlayerList.at(i).getName() == playerOneName)
            {
                m_MainPlayerList.at(i).addVPs(playerOneScore);
                m_MainPlayerList.at(i).addVPDiff(playerOneScore - playerTwoScore);
                AddResult(playerOneResult, i);
            }
            else if(m_MainPlayerList.at(i).getName() == playerTwoName)
            {
                m_MainPlayerList.at(i).addVPs(playerTwoScore);
                m_MainPlayerList.at(i).addVPDiff(playerTwoScore - playerOneScore);
                AddResult(playerTwoResult, i);
            }
        }
    }
}

void MainWindow::CreateRankedMatchup()
{
    UtilDebug("Create Ranked Matchup");
    m_CurrentRoundMatchups->clear();
    for(uint i = 0; i < m_MainPlayerList.size(); i += 2)
    {
        Player one = m_MainPlayerList.at(i);
        Player two = m_MainPlayerList.at(i + 1);

        m_CurrentRoundMatchups->push_back(std::pair<Player, Player>(one, two));
    }
    m_AllRoundMatchups->push_back(*m_CurrentRoundMatchups);
    UpdateMatchupsTable();
}
void MainWindow::UpdateRankings()
{
    UtilDebug("Update Rankings");
    std::vector<Player> tempList = m_MainPlayerList;
    std::vector<Player> tempRankings;
    Player highestRank;
    std::vector<Player>::iterator highestRankPlayer;

    for(uint i = 0; i < m_MainPlayerList.size(); ++i)
    {
        highestRankPlayer = tempList.begin();

        for(auto it = tempList.begin() + 1; it != tempList.end(); ++it)
        {
            if(it->getTPs() > highestRankPlayer->getTPs())
            {
                highestRankPlayer = it;
            }
            else if(it->getTPs() == highestRankPlayer->getTPs())
            {
                if(CheckTiebreakers(*highestRankPlayer, *it))
                {
                    highestRankPlayer = it;
                }
            }
        }

        tempRankings.push_back(*highestRankPlayer);
        tempList.erase(highestRankPlayer);
    }
    
    m_MainPlayerList = std::move(tempRankings);
    UpdatePlayerRankingList();
}

void MainWindow::ResetMatchupsTable()
{
    UtilDebug("Reset Matchups Table");
    ui->MatchupsPlayerTableWidget->clear();

    int i = ui->MatchupsPlayerTableWidget->rowCount();
    for(int j = 0; j < i; ++j)
    {
        ui->MatchupsPlayerTableWidget->removeRow(0);
    }

    ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Player One Name"));
    ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Player One Result"));
    ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Player One VPs"));
    ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Player Two Name"));
    ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Player Two Result"));
    ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Player Two VPs"));
}

void MainWindow::UpdateMatchupsTable()
{
    UtilDebug("Update Matchups Table");
    ResetMatchupsTable();

    for(uint i = 0; i < m_CurrentRoundMatchups->size(); ++i)
    {
        ui->MatchupsPlayerTableWidget->insertRow(ui->MatchupsPlayerTableWidget->rowCount());
        ui->MatchupsPlayerTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(m_CurrentRoundMatchups->at(i).first.getName())));
        ui->MatchupsPlayerTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString("")));
        ui->MatchupsPlayerTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString("")));
        ui->MatchupsPlayerTableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(m_CurrentRoundMatchups->at(i).second.getName())));
        ui->MatchupsPlayerTableWidget->setItem(i, 4, new QTableWidgetItem(QString::fromStdString("")));
        ui->MatchupsPlayerTableWidget->setItem(i, 5, new QTableWidgetItem(QString::fromStdString("")));
    }
}

void MainWindow::ResetRoundInput()
{
    UtilDebug("Reset Round Input");
    ui->MatchupsPlayerOneNameLabel->setText(QString("Player One"));
    ui->MatchupsPlayerOneComboBox->setCurrentIndex(0);
    ui->MatchupsPlayerOneSpinBox->setValue(0);
    ui->MatchupsPlayerTwoNameLabel->setText(QString("Player Two"));
    ui->MatchupsPlayerTwoComboBox->setCurrentIndex(0);
    ui->MatchupsPlayerTwoSpinBox->setValue(0);

    DisableRoundInput();
}

void MainWindow::DisableRoundInput()
{
    UtilDebug("Disable Round Input");
    ui->MatchupsPlayerOneComboBox->setEnabled(false);
    ui->MatchupsPlayerOneNameLabel->setEnabled(false);
    ui->MatchupsPlayerOneSpinBox->setEnabled(false);
    ui->MatchupsPlayerTwoComboBox->setEnabled(false);
    ui->MatchupsPlayerTwoNameLabel->setEnabled(false);
    ui->MatchupsPlayerTwoSpinBox->setEnabled(false);
    ui->MatchupsEnterResultsButton->setEnabled(false);
}

void MainWindow::EnableRoundInput()
{
    UtilDebug("Enable Round Input");
    ui->MatchupsPlayerOneComboBox->setEnabled(true);
    ui->MatchupsPlayerOneNameLabel->setEnabled(true);
    ui->MatchupsPlayerOneSpinBox->setEnabled(true);
    ui->MatchupsPlayerTwoComboBox->setEnabled(true);
    ui->MatchupsPlayerTwoNameLabel->setEnabled(true);
    ui->MatchupsPlayerTwoSpinBox->setEnabled(true);
    ui->MatchupsEnterResultsButton->setEnabled(true);
}

void MainWindow::UpdateRoundInput()
{
    UtilDebug("Update Round Input");
    EnableRoundInput();

    std::pair<Player, Player> tempMatchup;
    tempMatchup = m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow);

    ui->MatchupsPlayerOneNameLabel->setText(QString::fromStdString(tempMatchup.first.getName()));
    ui->MatchupsPlayerTwoNameLabel->setText(QString::fromStdString(tempMatchup.second.getName()));
}

bool MainWindow::IsRoundFinished() const
{
    for(int i = 0; i < ui->MatchupsPlayerTableWidget->rowCount(); ++i)
    {
        if(ui->MatchupsPlayerTableWidget->item(i, 1)->text() == QString::fromStdString("")
                || ui->MatchupsPlayerTableWidget->item(i, 4)->text() == QString::fromStdString(""))
        {
            return false;
        }
    }

    return true;
}

bool MainWindow::ResultsValid() const
{
    if(ui->MatchupsPlayerOneComboBox->currentText() == QString("Win")
            && ui->MatchupsPlayerTwoComboBox->currentText() == QString("Loss"))
    {
        if(!*m_TableIsVictory)
        {
            if(ui->MatchupsPlayerOneSpinBox->value() <= ui->MatchupsPlayerTwoSpinBox->value())
            {
                return false;
            }
        }
        return true;
    }
    else if(ui->MatchupsPlayerOneComboBox->currentText() == QString("Draw")
            && ui->MatchupsPlayerTwoComboBox->currentText() == QString("Draw"))
    {
        if(ui->MatchupsPlayerOneSpinBox->value() != ui->MatchupsPlayerTwoSpinBox->value())
        {
            return false;
        }
        return true;
    }
    else if(ui->MatchupsPlayerOneComboBox->currentText() == QString("Loss")
            && ui->MatchupsPlayerTwoComboBox->currentText() == QString("Win"))
    {
        if(!*m_TableIsVictory)
        {
            if(ui->MatchupsPlayerOneSpinBox->value() >= ui->MatchupsPlayerTwoSpinBox->value())
            {
                return false;
            }
        }
        return true;
    }

    return false;
}

void MainWindow::AddResult(const std::string &result, int const playerIndex)
{
    UtilLog("Add Result: " + result);
    if(result == "Win")
    {
        m_MainPlayerList.at(playerIndex).addWin(*m_WinValue);
    }
    else if(result == "Draw")
    {
        m_MainPlayerList.at(playerIndex).addDraw(*m_DrawValue);
    }
    else if(result == "Loss")
    {
        m_MainPlayerList.at(playerIndex).addLoss(*m_LossValue);
    }
}

void MainWindow::UpdateTableMatchUp(const Player &playerOne, const Player &playerTwo)
{
    UtilLog("Set Matchup: " + playerOne.getName() + " vs " + playerTwo.getName());
    for(uint i = 0; i < m_CurrentRoundMatchups->size(); ++i)
    {
        if(m_CurrentRoundMatchups->at(i).first.getName() == playerOne.getName())
        {
            m_CurrentRoundMatchups->at(i).second = playerTwo;
            break;
        }
        else if(m_CurrentRoundMatchups->at(i).second.getName() == playerOne.getName())
        {
            m_CurrentRoundMatchups->at(i).first = playerTwo;
            break;
        }
        else if(m_CurrentRoundMatchups->at(i).first.getName() == playerTwo.getName())
        {
            m_CurrentRoundMatchups->at(i).second = playerOne;
            break;
        }
        else if(m_CurrentRoundMatchups->at(i).second.getName() == playerTwo.getName())
        {
            m_CurrentRoundMatchups->at(i).first = playerOne;
            break;
        }
    }
}

Player MainWindow::FindRandomPlayer(const int playerCount)
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

bool MainWindow::CheckTiebreakers(const Player &playerOne, const Player &playerTwo) const
{
    //Calculate the result of the first Tiebreaker
    TiebreakerResult firstTiebreakerResult = CheckFirstTiebreaker(playerOne, playerTwo);
    if(firstTiebreakerResult == TiebreakerResult::HIGHER)
    {
        return true;
    }
    else if(firstTiebreakerResult == TiebreakerResult::EQUAL)
    {
        //Calculate the result of the second Tiebreaker
        TiebreakerResult secondTiebreakerResult = CheckSecondTiebreaker(playerOne, playerTwo);
        if(secondTiebreakerResult == TiebreakerResult::HIGHER)
        {
            return true;
        }
        else if(secondTiebreakerResult == TiebreakerResult::EQUAL)
        {
            //Calculate the result of the third Tiebreaker
            TiebreakerResult thirdTiebreakerResult = CheckThirdTiebreaker(playerOne, playerTwo);
            if(thirdTiebreakerResult == TiebreakerResult::HIGHER)
            {
                return true;
            }
            else if(thirdTiebreakerResult == TiebreakerResult::EQUAL)
            {
                //Calculate the result of the fourth Tiebreaker
                TiebreakerResult fourthTiebreakerResult = CheckFourthTiebreaker(playerOne, playerTwo);
                if(fourthTiebreakerResult == TiebreakerResult::HIGHER)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

TiebreakerResult MainWindow::CheckFirstTiebreaker(const Player &playerOne, const Player &playerTwo) const
{
    return CheckTiebreaker(m_FirstTiebreaker, playerOne, playerTwo);
}

TiebreakerResult MainWindow::CheckSecondTiebreaker(const Player &playerOne, const Player &playerTwo) const
{
    return CheckTiebreaker(m_SecondTiebreaker, playerOne, playerTwo);
}

TiebreakerResult MainWindow::CheckThirdTiebreaker(const Player &playerOne, const Player &playerTwo) const
{
    return CheckTiebreaker(m_ThirdTiebreaker, playerOne, playerTwo);
}

TiebreakerResult MainWindow::CheckFourthTiebreaker(const Player &playerOne, const Player &playerTwo) const
{
    return CheckTiebreaker(m_FourthTiebreaker, playerOne, playerTwo);
}

TiebreakerResult MainWindow::CheckTiebreaker(const Tiebreak::Tiebreaker tiebreak, const Player &playerOne, const Player &playerTwo) const
{
    switch (tiebreak) {
    case Tiebreak::Tiebreaker::VP_TOTAL:
        return CheckTiebreakerValue(playerOne.getVPs(), playerTwo.getVPs());
    case Tiebreak::Tiebreaker::VP_DIFF:
        return CheckTiebreakerValue(playerOne.getVPDiff(), playerTwo.getVPDiff());
    case Tiebreak::Tiebreaker::MOST_SPORTING:
        return CheckTiebreakerValue(playerOne.getMostSportingVotes(), playerTwo.getMostSportingVotes());
    case Tiebreak::Tiebreaker::BEST_PAINTED:
        return CheckTiebreakerValue(playerOne.getBestPaintedArmyVotes(), playerTwo.getBestPaintedArmyVotes());
    case Tiebreak::Tiebreaker::NONE:
    default:
        return TiebreakerResult::EQUAL;
    }
}

TiebreakerResult MainWindow::CheckTiebreakerValue(const int playerOne, const int playerTwo) const
{
    if(playerOne < playerTwo)
    {
        return TiebreakerResult::HIGHER;
    }
    else if(playerOne == playerTwo)
    {
        return TiebreakerResult::EQUAL;
    }
    else
    {
        return TiebreakerResult::LOWER;
    }
}
