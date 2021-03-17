#include "include/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::loadResultsPage()
{
    m_Ui->stackedWidget->setCurrentIndex(Pages::RESULTS_PAGE);

    populateComboBox(*m_Ui->bestPaintedVotedComboBox);
    populateComboBox(*m_Ui->bestPaintedVotingComboBox);
    populateComboBox(*m_Ui->mostSportingVotedComboBox);
    populateComboBox(*m_Ui->mostSportingVotingComboBox);

    resetPage();
}

void MainWindow::populateComboBox(QComboBox &comboBox)
{
    comboBox.addItem("");
    for(const auto &it : m_MainPlayerList)
    {
        comboBox.addItem(it.GetName().c_str());
    }
}

void MainWindow::on_bestPaintedPushButton_clicked()
{
    //Get Player Names
    std::string votingPlayer = m_Ui->bestPaintedVotingComboBox->currentText().toStdString();
    std::string votedPlayer = m_Ui->bestPaintedVotedComboBox->currentText().toStdString();
    UtilLog("Best Pained Vote: " + votingPlayer + " voting for " + votedPlayer);
    bool addVote = false;
    bool processVote = true;

    if(votingPlayer == "" || votedPlayer == "")
        processVote = false;

    if(votingPlayer == votedPlayer && processVote)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Voting for Themselves", "This player is voting for themselves. Do you wish to still process the vote?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No)
            processVote = false;
    }

    if(processVote)
    {
        //Check if Voting Player has Voted
        for(int i = 0; i < m_MainPlayerList.size(); ++i)
        {
            if(m_MainPlayerList.at(i).GetName() == votingPlayer)
            {
                if(m_MainPlayerList.at(i).HasVotedBestPainted())
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Previously Voted", "This player has already voted. Do you wish to replace their previous vote?", QMessageBox::Yes|QMessageBox::No);
                    if(reply == QMessageBox::Yes)
                    {
                        for(int j = 0; j < m_MainPlayerList.size(); ++j)
                        {
                            if(m_MainPlayerList.at(j).GetName() == m_MainPlayerList.at(i).GetBestPaintedVote())
                            {
                                m_MainPlayerList.at(j).RemoveBestPaintedArmyVote(m_BestPaintedValue);
                                break;
                            }
                        }
                        m_MainPlayerList.at(i).SetBestPaintedVote(votedPlayer);
                        addVote = true;
                        break;
                    }
                    else if(reply == QMessageBox::No)
                    {
                        addVote = false;
                        break;
                    }
                    else
                    {
                        addVote = false;
                        break;
                    }
                }
                else
                {
                    addVote = true;
                    m_MainPlayerList.at(i).SetVotedBestPainted(true);
                    m_MainPlayerList.at(i).SetBestPaintedVote(votedPlayer);
                    break;
                }

            }
        }

        if(addVote)
        {
            for(int i = 0; i < m_MainPlayerList.size(); ++i)
            {
                if(m_MainPlayerList.at(i).GetName() == votedPlayer)
                {
                    UtilLog("Adding Best Painted Vote");
                    m_MainPlayerList.at(i).AddBestPaintedArmyVote(m_BestPaintedValue);
                    m_NumBestPaintedVotes++;
                    break;
                }
            }
        }
    }

    //clear field
    m_Ui->bestPaintedVotedComboBox->setCurrentIndex(0);
    m_Ui->bestPaintedVotingComboBox->setCurrentIndex(0);
}

void MainWindow::on_mostSportingPushButton_clicked()
{
    //Get Player Names
    std::string votingPlayer = m_Ui->mostSportingVotingComboBox->currentText().toStdString();
    std::string votedPlayer = m_Ui->mostSportingVotedComboBox->currentText().toStdString();
    UtilLog("Most Sporting: " + votingPlayer + " voting for " + votedPlayer);
    bool addVote = false;
    bool processVote = true;

    if(votingPlayer == "" || votedPlayer == "")
        processVote = false;

    if(votingPlayer == votedPlayer)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Voting for Themselves", "This player is voting for themselves. Do you wish to still process the vote?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No)
            processVote = false;
    }

    if(processVote)
    {
        //Check if Voting Player has Voted
        for(int i = 0; i < m_MainPlayerList.size(); ++i)
        {
            if(m_MainPlayerList.at(i).GetName() == votingPlayer)
            {
                if(m_MainPlayerList.at(i).HasVotedMostSporting())
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Previously Voted", "This player has already voted. Do you wish to replace their previous vote?", QMessageBox::Yes|QMessageBox::No);
                    if(reply == QMessageBox::Yes)
                    {
                        for(int j = 0; j < m_MainPlayerList.size(); ++j)
                        {
                            if(m_MainPlayerList.at(j).GetName() == m_MainPlayerList.at(i).GetMostSportingVote())
                            {
                                m_MainPlayerList.at(j).RemoveMostSportingVote(m_MostSportingValue);
                                break;
                            }
                        }
                        m_MainPlayerList.at(i).SetMostSportingVote(votedPlayer);
                        addVote = true;
                        break;
                    }
                    else if(reply == QMessageBox::No)
                    {
                        addVote = false;
                        break;
                    }
                    else
                    {
                        addVote = false;
                        break;
                    }
                }
            }
            else
            {
                addVote = true;
                m_MainPlayerList.at(i).SetVotedMostSporting(true);
                m_MainPlayerList.at(i).SetMostSportingVote(votedPlayer);
                break;
            }
        }

        if(addVote)
        {
            for(int i = 0; i < m_MainPlayerList.size(); ++i)
            {
                if(m_MainPlayerList.at(i).GetName() == votedPlayer)
                {
                    UtilLog("Most Sportin Vote Added");
                    m_MainPlayerList.at(i).AddMostSportingVote(m_MostSportingValue);
                    m_NumMostSportingVotes++;
                    break;
                }
            }
        }
    }


    //clear field
    m_Ui->mostSportingVotedComboBox->setCurrentIndex(0);
    m_Ui->mostSportingVotingComboBox->setCurrentIndex(0);
}

void MainWindow::on_displayResultsPushButton_clicked()
{
    UtilDebug("Display Results Clicked");
    bool display = true;
    if(m_NumBestPaintedVotes < m_MainPlayerList.size())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Missing Best Painted Votes", "Not all players have voted for Best Painted army, do you wish to continue?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            display = false;
        }
    }
    if(m_NumMostSportingVotes < m_MainPlayerList.size() && display)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Missing Most Sporting Votes", "Not all players have voted for Most Sporting, do you wish to continue?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            display = false;
        }
    }
    if(display)
    {
        UtilLog("Displaying Results");
        resetPage();
        UpdateRankings();

        //Fill in all award data
        std::string mostSportingPlayer = getMostSportingPlayer();
        std::string bestPaintedPlayer = getBestPaintedPlayer();
        std::string woodenSpoonPlayer = getWoodenSpoonPlayer();
        std::vector<std::string> podiumPlayers = getPodiumPlayers();

        m_Ui->mostSportingInputLabel->setText(mostSportingPlayer.c_str());
        m_Ui->bestPaintedInputLabel->setText(bestPaintedPlayer.c_str());
        m_Ui->woodenSpoonInputLabel->setText(woodenSpoonPlayer.c_str());
        m_Ui->firstPlaceInputLabel->setText(podiumPlayers.at(0).c_str());
        m_Ui->secondPlaceInputLabel->setText(podiumPlayers.at(1).c_str());
        m_Ui->thirdPlaceInputLabel->setText(podiumPlayers.at(2).c_str());

        populateResultsTable();
    }
    else
        UtilLog("Results not being displayed");
}

std::string MainWindow::getMostSportingPlayer()
{
    UtilLog("Get Most Sporting Player");
    std::string mostSportingPlayer;
    int mostSportingVotes = 0;

    for(const auto &player : m_MainPlayerList)
    {
        if(player.GetMostSportingVotes() > mostSportingVotes)
        {
            mostSportingVotes = player.GetMostSportingVotes();
            mostSportingPlayer = player.GetName();
        }
    }

    return mostSportingPlayer;
}

std::string MainWindow::getBestPaintedPlayer()
{
    UtilLog("Get Best Painted Player");
    std::string bestPaintedPlayer;
    int bestPaintedVotes = 0;

    for(const auto &player : m_MainPlayerList)
    {
        if(player.GetBestPaintedArmyVotes() > bestPaintedVotes)
        {
            bestPaintedVotes = player.GetBestPaintedArmyVotes();
            bestPaintedPlayer = player.GetName();
        }
    }

    return bestPaintedPlayer;
}

std::string MainWindow::getWoodenSpoonPlayer()
{
    UtilLog("Get Wooden Spoon Player");
    return m_MainPlayerList.at(m_MainPlayerList.size() - 1).GetName();
}

std::vector<std::string> MainWindow::getPodiumPlayers()
{
    UtilLog("Get Podium Players");
    std::vector<std::string> podiumPlayers;
    podiumPlayers.push_back(m_MainPlayerList.at(0).GetName());
    podiumPlayers.push_back(m_MainPlayerList.at(1).GetName());
    podiumPlayers.push_back(m_MainPlayerList.at(2).GetName());

    return podiumPlayers;
}

void MainWindow::populateResultsTable()
{
    UtilDebug("Populate Results Table");
    for(const auto &player : m_MainPlayerList)
    {
        int rowCount = m_Ui->fullResultsTable->rowCount();
        m_Ui->fullResultsTable->insertRow(rowCount);
        m_Ui->fullResultsTable->setItem(rowCount, 0, new QTableWidgetItem(IntToQString(rowCount + 1)));
        m_Ui->fullResultsTable->setItem(rowCount, 1, new QTableWidgetItem(player.GetName().c_str()));
        m_Ui->fullResultsTable->setItem(rowCount, 2, new QTableWidgetItem(IntToQString(player.GetTPs())));
        m_Ui->fullResultsTable->setItem(rowCount, 3, new QTableWidgetItem(IntToQString(player.GetVPs())));
        m_Ui->fullResultsTable->setItem(rowCount, 4, new QTableWidgetItem(IntToQString(player.GetVPDiff())));
        m_Ui->fullResultsTable->setItem(rowCount, 5, new QTableWidgetItem(IntToQString(player.GetBestPaintedArmyVotes())));
        m_Ui->fullResultsTable->setItem(rowCount, 6, new QTableWidgetItem(IntToQString(player.GetMostSportingVotes())));
    }
}

void MainWindow::resetPage()
{
    m_Ui->firstPlaceInputLabel->clear();
    m_Ui->secondPlaceInputLabel->clear();
    m_Ui->thirdPlaceInputLabel->clear();
    m_Ui->woodenSpoonInputLabel->clear();
    m_Ui->mostSportingInputLabel->clear();
    m_Ui->bestPaintedInputLabel->clear();

    m_Ui->bestPaintedInputLabel->setText("Not Set");
    m_Ui->bestPaintedVotedComboBox->setCurrentIndex(0);
    m_Ui->bestPaintedVotingComboBox->setCurrentIndex(0);

    m_Ui->mostSportingInputLabel->setText("Not Set");
    m_Ui->mostSportingVotedComboBox->setCurrentIndex(0);
    m_Ui->mostSportingVotingComboBox->setCurrentIndex(0);

    m_Ui->firstPlaceInputLabel->setText("Not Set");
    m_Ui->secondPlaceInputLabel->setText("Not Set");
    m_Ui->thirdPlaceInputLabel->setText("Not Set");
    m_Ui->woodenSpoonInputLabel->setText("Not Set");

    m_Ui->fullResultsTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Position"));
    m_Ui->fullResultsTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Player Name"));
    m_Ui->fullResultsTable->setHorizontalHeaderItem(2, new QTableWidgetItem("TPs"));
    m_Ui->fullResultsTable->setHorizontalHeaderItem(3, new QTableWidgetItem("VPs"));
    m_Ui->fullResultsTable->setHorizontalHeaderItem(4, new QTableWidgetItem("VP Difference"));
    m_Ui->fullResultsTable->setHorizontalHeaderItem(5, new QTableWidgetItem("Best Painted"));
    m_Ui->fullResultsTable->setHorizontalHeaderItem(6, new QTableWidgetItem("Most Sporting"));
}
