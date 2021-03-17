#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "time.h"
#include <set>

// This .cpp file is used to handle the Matchups Page

void MainWindow::AddResult(const std::string &result, int const playerIndex)
{
    UtilLog("Add Result: " + result);
    if(result == "Win")
    {
        m_MainPlayerList.at(playerIndex).AddWin(*m_WinValue);
    }
    else if(result == "Draw")
    {
        m_MainPlayerList.at(playerIndex).AddDraw(*m_DrawValue);
    }
    else if(result == "Loss")
    {
        m_MainPlayerList.at(playerIndex).AddLoss(*m_LossValue);
    }
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
        return CheckTiebreakerValue(playerOne.GetVPs(), playerTwo.GetVPs());
    case Tiebreak::Tiebreaker::VP_DIFF:
        return CheckTiebreakerValue(playerOne.GetVPDiff(), playerTwo.GetVPDiff());
    case Tiebreak::Tiebreaker::MOST_SPORTING:
        return CheckTiebreakerValue(playerOne.GetMostSportingVotes(), playerTwo.GetMostSportingVotes());
    case Tiebreak::Tiebreaker::BEST_PAINTED:
        return CheckTiebreakerValue(playerOne.GetBestPaintedArmyVotes(), playerTwo.GetBestPaintedArmyVotes());
    case Tiebreak::Tiebreaker::NONE:
    default:
        return TiebreakerResult::EQUAL;
    }
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

void MainWindow::CreateInitialMatchups()
{
    UtilDebug("Set Initial Matchups");
    if(m_MainPlayerList.size() % 2 != 0)
    {
        UtilLog("Added Ringer");
        m_MainPlayerList.emplace_back("Ringer", 0);
    }

    m_UsingSeed ? CreateSeededMatchup() : CreateRandomMatchup();
}

void MainWindow::CreateRandomMatchup()
{
    UtilDebug("Create Random Matchups");
    int playerNumber = m_MainPlayerList.size();

    for(auto &it : m_MainPlayerList)
    {
        it.SetFirstRoundSet(false);
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

void MainWindow::CreateSeededMatchup()
{
    UtilDebug("Create Seeded Matchups");

    auto playerSeedComparison = [](Player one, Player two){
        return one.GetSeed() < two.GetSeed() ? true : false;
    };
    std::set<Player, decltype(playerSeedComparison)> temporarySet(playerSeedComparison);
    for(auto &player : m_MainPlayerList)
    {
        player.SetFirstRoundSet(false);
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

void MainWindow::DisableRoundInput()
{
    UtilDebug("Disable Round Input");
    m_Ui->MatchupsPlayerOneComboBox->setEnabled(false);
    m_Ui->MatchupsPlayerOneNameLabel->setEnabled(false);
    m_Ui->MatchupsPlayerOneSpinBox->setEnabled(false);
    m_Ui->MatchupsPlayerTwoComboBox->setEnabled(false);
    m_Ui->MatchupsPlayerTwoNameLabel->setEnabled(false);
    m_Ui->MatchupsPlayerTwoSpinBox->setEnabled(false);
    m_Ui->MatchupsEnterResultsButton->setEnabled(false);
}

void MainWindow::EnableRoundInput()
{
    UtilDebug("Enable Round Input");
    m_Ui->MatchupsPlayerOneComboBox->setEnabled(true);
    m_Ui->MatchupsPlayerOneNameLabel->setEnabled(true);
    m_Ui->MatchupsPlayerOneSpinBox->setEnabled(true);
    m_Ui->MatchupsPlayerTwoComboBox->setEnabled(true);
    m_Ui->MatchupsPlayerTwoNameLabel->setEnabled(true);
    m_Ui->MatchupsPlayerTwoSpinBox->setEnabled(true);
    m_Ui->MatchupsEnterResultsButton->setEnabled(true);
}

Player MainWindow::FindRandomPlayer(const int playerCount)
{
    srand(time(NULL));

    bool searching = true;
    int number;
    while(searching)
    {
        number = rand() % playerCount;
        if(!m_MainPlayerList.at(number).IsFirstRoundSet())
        {
            m_MainPlayerList.at(number).SetFirstRoundSet(true);
            searching = false;
        }
    }
    return m_MainPlayerList.at(number);
}

bool MainWindow::IsRoundFinished() const
{
    for(int i = 0; i < m_Ui->MatchupsPlayerTableWidget->rowCount(); ++i)
    {
        if(m_Ui->MatchupsPlayerTableWidget->item(i, 1)->text() == QString::fromStdString("")
                || m_Ui->MatchupsPlayerTableWidget->item(i, 4)->text() == QString::fromStdString(""))
        {
            return false;
        }
    }

    return true;
}

void MainWindow::LoadMatchupsPage()
{
    UtilDebug("Load Matchups Page");
    m_Ui->stackedWidget->setCurrentIndex(Pages::MATCHUPS_PAGE);
    m_Ui->MatchupsPreviousRoundButton->setEnabled(false);
    m_Ui->MatchupsCurrentRoundButton->setEnabled(false);
    m_Ui->MatchupsModifyMatchupsButton->setEnabled(false);
    m_CurrentRoundNumber = 1;
    m_Ui->MatchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));
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

void MainWindow::LoadMatchupsPageFromLoadedEvent()
{
    UtilDebug("Load Matchups Page from Loaded Event");
    m_Ui->stackedWidget->setCurrentIndex(Pages::MATCHUPS_PAGE);
    m_Ui->MatchupsPreviousRoundButton->setEnabled(false);
    m_Ui->MatchupsCurrentRoundButton->setEnabled(false);
    m_Ui->MatchupsModifyMatchupsButton->setEnabled(false);
    m_Ui->MatchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));

    ResetRoundInput();

    UtilDebug("Event Matchups Loaded");
    *m_CurrentRoundMatchups = m_AllRoundMatchups->at(m_CurrentRoundNumber - 1);

    UpdateMatchupsTable();

    //List Players
    m_Ui->MatchupsPlayerListWidget->clear();

    for(const auto &player : m_MainPlayerList)
    {
        m_Ui->MatchupsPlayerListWidget->addItem(QString::fromStdString(player.GetName()));
    }
}

void MainWindow::NewMatchUpsFromSwap(const Player &playerOne, const Player &playerTwo, const Player &playerThree, const Player &playerFour)
{
    UpdateTableMatchUp(playerOne, playerTwo);
    UpdateTableMatchUp(playerThree, playerFour);
    UpdateMatchupsTable();
}

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
        if(m_Ui->MatchupsPlayerTableWidget->item(*m_MatchupsSelectedRow, 0)->text() == m_Ui->MatchupsPlayerOneNameLabel->text()
                && m_Ui->MatchupsPlayerTableWidget->item(*m_MatchupsSelectedRow, 3)->text() == m_Ui->MatchupsPlayerTwoNameLabel->text())
        {
            m_Ui->MatchupsPlayerTableWidget->setItem(*m_MatchupsSelectedRow, 1, new QTableWidgetItem(m_Ui->MatchupsPlayerOneComboBox->currentText()));
            m_Ui->MatchupsPlayerTableWidget->setItem(*m_MatchupsSelectedRow, 2, new QTableWidgetItem(m_Ui->MatchupsPlayerOneSpinBox->text()));
            m_Ui->MatchupsPlayerTableWidget->setItem(*m_MatchupsSelectedRow, 4, new QTableWidgetItem(m_Ui->MatchupsPlayerTwoComboBox->currentText()));
            m_Ui->MatchupsPlayerTableWidget->setItem(*m_MatchupsSelectedRow, 5, new QTableWidgetItem(m_Ui->MatchupsPlayerTwoSpinBox->text()));
        }
        else
        {
            UtilLog("Error: Matchup doesnt match");
            UtilLog(m_Ui->MatchupsPlayerTableWidget->item(*m_MatchupsSelectedRow, 0)->text().toStdString());
            UtilLog(m_Ui->MatchupsPlayerOneNameLabel->text().toStdString());
            UtilLog(m_Ui->MatchupsPlayerTableWidget->item(*m_MatchupsSelectedRow, 3)->text().toStdString());
            UtilLog(m_Ui->MatchupsPlayerTwoNameLabel->text().toStdString());
        }

        ResetRoundInput();
        m_Ui->MatchupsModifyMatchupsButton->setEnabled(false);
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

    if(m_Ui->MatchupsPlayerTableWidget->rowCount() > *m_MatchupsSelectedRow + 1)
    {
        UtilLog(std::to_string(m_Ui->MatchupsPlayerTableWidget->rowCount()));
        UtilLog(std::to_string(*m_MatchupsSelectedRow));

        Player playerOne = m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow).second;
        Player playerTwo = m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow + 1).first;

        m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow).second.operator=(playerTwo);
        m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow + 1).first.operator=(playerOne);

        UpdateMatchupsTable();
        ResetRoundInput();
        m_Ui->MatchupsModifyMatchupsButton->setEnabled(false);
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

        m_Ui->MatchupsCurrentRoundNumberLabel->setText(QString::fromStdString("Current Round Number: " + std::to_string(m_CurrentRoundNumber)));
        UtilLog("Round " + std::to_string(m_CurrentRoundNumber) + " Started");
    }
    else
    {
        QMessageBox::warning(this, "Unfinished Round", "Round has not finished. Please make sure that all matchup data has been set.");
    }

    SaveEventData();

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

void MainWindow::ResetMatchupsTable()
{
    UtilDebug("Reset Matchups Table");
    m_Ui->MatchupsPlayerTableWidget->clear();

    int i = m_Ui->MatchupsPlayerTableWidget->rowCount();
    for(int j = 0; j < i; ++j)
    {
        m_Ui->MatchupsPlayerTableWidget->removeRow(0);
    }

    m_Ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Player One Name"));
    m_Ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Player One Result"));
    m_Ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Player One VPs"));
    m_Ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Player Two Name"));
    m_Ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Player Two Result"));
    m_Ui->MatchupsPlayerTableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Player Two VPs"));
}

void MainWindow::ResetRoundInput()
{
    UtilDebug("Reset Round Input");
    m_Ui->MatchupsPlayerOneNameLabel->setText(QString("Player One"));
    m_Ui->MatchupsPlayerOneComboBox->setCurrentIndex(0);
    m_Ui->MatchupsPlayerOneSpinBox->setValue(0);
    m_Ui->MatchupsPlayerTwoNameLabel->setText(QString("Player Two"));
    m_Ui->MatchupsPlayerTwoComboBox->setCurrentIndex(0);
    m_Ui->MatchupsPlayerTwoSpinBox->setValue(0);

    DisableRoundInput();
}

bool MainWindow::ResultsValid() const
{
    if(m_Ui->MatchupsPlayerOneComboBox->currentText() == QString("Win")
            && m_Ui->MatchupsPlayerTwoComboBox->currentText() == QString("Loss"))
    {
        if(!*m_TableIsVictory)
        {
            if(m_Ui->MatchupsPlayerOneSpinBox->value() <= m_Ui->MatchupsPlayerTwoSpinBox->value())
            {
                return false;
            }
        }
        return true;
    }
    else if(m_Ui->MatchupsPlayerOneComboBox->currentText() == QString("Draw")
            && m_Ui->MatchupsPlayerTwoComboBox->currentText() == QString("Draw"))
    {
        if(m_Ui->MatchupsPlayerOneSpinBox->value() != m_Ui->MatchupsPlayerTwoSpinBox->value())
        {
            return false;
        }
        return true;
    }
    else if(m_Ui->MatchupsPlayerOneComboBox->currentText() == QString("Loss")
            && m_Ui->MatchupsPlayerTwoComboBox->currentText() == QString("Win"))
    {
        if(!*m_TableIsVictory)
        {
            if(m_Ui->MatchupsPlayerOneSpinBox->value() >= m_Ui->MatchupsPlayerTwoSpinBox->value())
            {
                return false;
            }
        }
        return true;
    }

    return false;
}

void MainWindow::UpdateMatchupsTable()
{
    UtilDebug("Update Matchups Table");
    ResetMatchupsTable();

    for(uint i = 0; i < m_CurrentRoundMatchups->size(); ++i)
    {
        m_Ui->MatchupsPlayerTableWidget->insertRow(m_Ui->MatchupsPlayerTableWidget->rowCount());
        m_Ui->MatchupsPlayerTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(m_CurrentRoundMatchups->at(i).first.GetName())));
        m_Ui->MatchupsPlayerTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString("")));
        m_Ui->MatchupsPlayerTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString("")));
        m_Ui->MatchupsPlayerTableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(m_CurrentRoundMatchups->at(i).second.GetName())));
        m_Ui->MatchupsPlayerTableWidget->setItem(i, 4, new QTableWidgetItem(QString::fromStdString("")));
        m_Ui->MatchupsPlayerTableWidget->setItem(i, 5, new QTableWidgetItem(QString::fromStdString("")));
    }
}

void MainWindow::UpdatePlayerRankingList()
{
    UtilDebug("Update Player Rankings List");
    m_Ui->MatchupsPlayerListWidget->clear();

    for(const auto &player : m_MainPlayerList)
    {
        m_Ui->MatchupsPlayerListWidget->addItem(QString::fromStdString(player.GetName()));
    }
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
            if(it->GetTPs() > highestRankPlayer->GetTPs())
            {
                highestRankPlayer = it;
            }
            else if(it->GetTPs() == highestRankPlayer->GetTPs())
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

void MainWindow::UpdateRoundInput()
{
    UtilDebug("Update Round Input");
    EnableRoundInput();

    std::pair<Player, Player> tempMatchup;
    tempMatchup = m_CurrentRoundMatchups->at(*m_MatchupsSelectedRow);

    m_Ui->MatchupsPlayerOneNameLabel->setText(QString::fromStdString(tempMatchup.first.GetName()));
    m_Ui->MatchupsPlayerTwoNameLabel->setText(QString::fromStdString(tempMatchup.second.GetName()));
}

void MainWindow::UpdateScores()
{
    UtilDebug("Update Scores");
    int rowCount = m_Ui->MatchupsPlayerTableWidget->rowCount();
    
    //For each row in the table
    for(int i = 0; i < rowCount; ++i)
    {
        //Player one and player two
        std::string playerOneName = m_Ui->MatchupsPlayerTableWidget->item(i, 0)->text().toStdString();
        std::string playerTwoName = m_Ui->MatchupsPlayerTableWidget->item(i, 3)->text().toStdString();
        
        int playerOneScore = m_Ui->MatchupsPlayerTableWidget->item(i, 2)->text().toInt();
        int playerTwoScore = m_Ui->MatchupsPlayerTableWidget->item(i, 5)->text().toInt();
        std::string playerOneResult = m_Ui->MatchupsPlayerTableWidget->item(i, 1)->text().toStdString();
        std::string playerTwoResult = m_Ui->MatchupsPlayerTableWidget->item(i, 4)->text().toStdString();
        
        //Find them in the player list
        for(uint i = 0; i < m_MainPlayerList.size(); ++i)
        {
            //Update the players values.
            if(m_MainPlayerList.at(i).GetName() == playerOneName)
            {
                m_MainPlayerList.at(i).AddVPs(playerOneScore);
                m_MainPlayerList.at(i).AddVPDiff(playerOneScore - playerTwoScore);
                AddResult(playerOneResult, i);
            }
            else if(m_MainPlayerList.at(i).GetName() == playerTwoName)
            {
                m_MainPlayerList.at(i).AddVPs(playerTwoScore);
                m_MainPlayerList.at(i).AddVPDiff(playerTwoScore - playerOneScore);
                AddResult(playerTwoResult, i);
            }
        }
    }
}

void MainWindow::UpdateTableMatchUp(const Player &playerOne, const Player &playerTwo)
{
    UtilLog("Set Matchup: " + playerOne.GetName() + " vs " + playerTwo.GetName());
    for(uint i = 0; i < m_CurrentRoundMatchups->size(); ++i)
    {
        if(m_CurrentRoundMatchups->at(i).first.GetName() == playerOne.GetName())
        {
            m_CurrentRoundMatchups->at(i).second = playerTwo;
            break;
        }
        else if(m_CurrentRoundMatchups->at(i).second.GetName() == playerOne.GetName())
        {
            m_CurrentRoundMatchups->at(i).first = playerTwo;
            break;
        }
        else if(m_CurrentRoundMatchups->at(i).first.GetName() == playerTwo.GetName())
        {
            m_CurrentRoundMatchups->at(i).second = playerOne;
            break;
        }
        else if(m_CurrentRoundMatchups->at(i).second.GetName() == playerTwo.GetName())
        {
            m_CurrentRoundMatchups->at(i).first = playerOne;
            break;
        }
    }
}
