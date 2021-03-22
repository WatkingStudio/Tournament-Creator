#include "include/mainwindow.h"
#include "ui_mainwindow.h"

// This .cpp file is used to handle the results page

std::string MainWindow::GetBestPaintedPlayer() const
{
    UtilLog("Get Best Painted Player");
    std::string bestPaintedPlayer;
    int bestPaintedVotes = 0;

    for(const auto &player : *m_MainPlayerList)
    {
        if(player.GetBestPaintedArmyVotes() > bestPaintedVotes)
        {
            bestPaintedVotes = player.GetBestPaintedArmyVotes();
            bestPaintedPlayer = player.GetName();
        }
    }

    return bestPaintedPlayer;
}

std::string MainWindow::GetMostSportingPlayer() const
{
    UtilLog("Get Most Sporting Player");
    std::string mostSportingPlayer;
    int mostSportingVotes = 0;

    for(const auto &player : *m_MainPlayerList)
    {
        if(player.GetMostSportingVotes() > mostSportingVotes)
        {
            mostSportingVotes = player.GetMostSportingVotes();
            mostSportingPlayer = player.GetName();
        }
    }

    return mostSportingPlayer;
}

PodiumPlayers MainWindow::GetPodiumPlayers() const
{
    UtilLog("Get Podium Players");
    PodiumPlayers podiumPlayers;
    podiumPlayers.PlayerOne = m_MainPlayerList->at(0).GetName();
    podiumPlayers.PlayerTwo = m_MainPlayerList->at(1).GetName();
    podiumPlayers.PlayerThree = m_MainPlayerList->at(2).GetName();

    return podiumPlayers;
}

std::string MainWindow::GetWoodenSpoonPlayer() const
{
    UtilLog("Get Wooden Spoon Player");
    return m_MainPlayerList->at(m_MainPlayerList->size() - 1).GetName();
}

void MainWindow::LoadResultsPage()
{
    m_Ui->StackedWidget->setCurrentIndex(Pages::RESULTS_PAGE);

    PopulateComboBox(*m_Ui->BestPaintedVotedComboBox);
    PopulateComboBox(*m_Ui->BestPaintedVotingComboBox);
    PopulateComboBox(*m_Ui->MostSportingVotedComboBox);
    PopulateComboBox(*m_Ui->MostSportingVotingComboBox);

    ResetPage();
}

void MainWindow::on_BestPaintedPushButton_clicked()
{
    //Get Player Names
    std::string votingPlayer = m_Ui->BestPaintedVotingComboBox->currentText().toStdString();
    std::string votedPlayer = m_Ui->BestPaintedVotedComboBox->currentText().toStdString();
    UtilLog("Best Pained Vote: " + votingPlayer + " voting for " + votedPlayer);
    bool addVote = false;
    bool processVote = true;

    if(votingPlayer == ""
            || votedPlayer == "")
    {
        processVote = false;
    }

    if(votingPlayer == votedPlayer
            && processVote)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Voting for Themselves", "This player is voting for themselves. Do you wish to still process the vote?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            processVote = false;
        }
    }

    if(processVote)
    {
        //Check if Voting Player has Voted
        for(uint i = 0; i < m_MainPlayerList->size(); ++i)
        {
            if(m_MainPlayerList->at(i).GetName() == votingPlayer)
            {
                if(m_MainPlayerList->at(i).HasVotedBestPainted())
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Previously Voted", "This player has already voted. Do you wish to replace their previous vote?", QMessageBox::Yes|QMessageBox::No);
                    if(reply == QMessageBox::Yes)
                    {
                        for(uint j = 0; j < m_MainPlayerList->size(); ++j)
                        {
                            if(m_MainPlayerList->at(j).GetName() == m_MainPlayerList->at(i).GetBestPaintedVote())
                            {
                                m_MainPlayerList->at(j).RemoveBestPaintedArmyVote(*m_BestPaintedValue);
                                break;
                            }
                        }
                        m_MainPlayerList->at(i).SetBestPaintedVote(votedPlayer);
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
                    m_MainPlayerList->at(i).SetVotedBestPainted(true);
                    m_MainPlayerList->at(i).SetBestPaintedVote(votedPlayer);
                    break;
                }
            }
        }

        if(addVote)
        {
            for(uint i = 0; i < m_MainPlayerList->size(); ++i)
            {
                if(m_MainPlayerList->at(i).GetName() == votedPlayer)
                {
                    UtilLog("Adding Best Painted Vote");
                    m_MainPlayerList->at(i).AddBestPaintedArmyVote(*m_BestPaintedValue);
                    (*m_NumBestPaintedVotes)++;
                    break;
                }
            }
        }
    }
    else
    {
        UtilLog("Best Painted vote not valid, vote not provessed.");
    }

    //clear field
    m_Ui->BestPaintedVotedComboBox->setCurrentIndex(0);
    m_Ui->BestPaintedVotingComboBox->setCurrentIndex(0);
}

void MainWindow::on_DisplayResultsPushButton_clicked()
{
    UtilDebug("Display Results Clicked");
    bool display = true;
    if(*m_NumBestPaintedVotes < m_MainPlayerList->size())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Missing Best Painted Votes", "Not all players have voted for Best Painted army, do you wish to continue?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            display = false;
        }
    }
    if(*m_NumMostSportingVotes < m_MainPlayerList->size()
            && display)
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
        ResetPage();
        UpdateRankings();

        //Fill in all award data
        std::string mostSportingPlayer = GetMostSportingPlayer();
        std::string bestPaintedPlayer = GetBestPaintedPlayer();
        std::string woodenSpoonPlayer = GetWoodenSpoonPlayer();
        PodiumPlayers podiumPlayers = GetPodiumPlayers();

        m_Ui->MostSportingInputLabel->setText(mostSportingPlayer.c_str());
        m_Ui->BestPaintedInputLabel->setText(bestPaintedPlayer.c_str());
        m_Ui->WoodenSpoonInputLabel->setText(woodenSpoonPlayer.c_str());
        m_Ui->FirstPlaceInputLabel->setText(podiumPlayers.PlayerOne.c_str());
        m_Ui->SecondPlaceInputLabel->setText(podiumPlayers.PlayerTwo.c_str());
        m_Ui->ThirdPlaceInputLabel->setText(podiumPlayers.PlayerThree.c_str());

        PopulateResultsTable();
    }
    else
    {
        UtilLog("Results not being displayed");
    }
}

void MainWindow::on_MostSportingPushButton_clicked()
{
    //Get Player Names
    std::string votingPlayer = m_Ui->MostSportingVotingComboBox->currentText().toStdString();
    std::string votedPlayer = m_Ui->MostSportingVotedComboBox->currentText().toStdString();
    UtilLog("Most Sporting: " + votingPlayer + " voting for " + votedPlayer);
    bool addVote = false;
    bool processVote = true;

    if(votingPlayer == ""
            || votedPlayer == "")
    {
        processVote = false;
    }

    if(votingPlayer == votedPlayer
            && processVote)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Voting for Themselves", "This player is voting for themselves. Do you wish to still process the vote?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No)
        {
            processVote = false;
        }
    }

    if(processVote)
    {
        //Check if Voting Player has Voted
        for(uint i = 0; i < m_MainPlayerList->size(); ++i)
        {
            if(m_MainPlayerList->at(i).GetName() == votingPlayer)
            {
                if(m_MainPlayerList->at(i).HasVotedMostSporting())
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Previously Voted", "This player has already voted. Do you wish to replace their previous vote?", QMessageBox::Yes|QMessageBox::No);
                    if(reply == QMessageBox::Yes)
                    {
                        for(uint j = 0; j < m_MainPlayerList->size(); ++j)
                        {
                            if(m_MainPlayerList->at(j).GetName() == m_MainPlayerList->at(i).GetMostSportingVote())
                            {
                                m_MainPlayerList->at(j).RemoveMostSportingVote(*m_MostSportingValue);
                                break;
                            }
                        }
                        m_MainPlayerList->at(i).SetMostSportingVote(votedPlayer);
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
                    m_MainPlayerList->at(i).SetVotedMostSporting(true);
                    m_MainPlayerList->at(i).SetMostSportingVote(votedPlayer);
                    break;
                }
            }
        }

        if(addVote)
        {
            for(uint i = 0; i < m_MainPlayerList->size(); ++i)
            {
                if(m_MainPlayerList->at(i).GetName() == votedPlayer)
                {
                    UtilLog("Most Sportin Vote Added");
                    m_MainPlayerList->at(i).AddMostSportingVote(*m_MostSportingValue);
                    (*m_NumMostSportingVotes)++;
                    break;
                }
            }
        }
    }
    else
    {
        UtilLog("Most Sporting vote invalid, vote not processed.");
    }

    //clear field
    m_Ui->MostSportingVotedComboBox->setCurrentIndex(0);
    m_Ui->MostSportingVotingComboBox->setCurrentIndex(0);
}

void MainWindow::PopulateComboBox(QComboBox &comboBox)
{
    comboBox.addItem("");
    for(const auto &it : *m_MainPlayerList)
    {
        comboBox.addItem(it.GetName().c_str());
    }
}

void MainWindow::PopulateResultsTable()
{
    UtilDebug("Populate Results Table");
    for(const auto &player : *m_MainPlayerList)
    {
        int rowCount = m_Ui->FullResultsTable->rowCount();
        m_Ui->FullResultsTable->insertRow(rowCount);
        m_Ui->FullResultsTable->setItem(rowCount, 0, new QTableWidgetItem(IntToQString(rowCount + 1)));
        m_Ui->FullResultsTable->setItem(rowCount, 1, new QTableWidgetItem(player.GetName().c_str()));
        m_Ui->FullResultsTable->setItem(rowCount, 2, new QTableWidgetItem(IntToQString(player.GetTPs())));
        m_Ui->FullResultsTable->setItem(rowCount, 3, new QTableWidgetItem(IntToQString(player.GetVPs())));
        m_Ui->FullResultsTable->setItem(rowCount, 4, new QTableWidgetItem(IntToQString(player.GetVPDiff())));
        m_Ui->FullResultsTable->setItem(rowCount, 5, new QTableWidgetItem(IntToQString(player.GetBestPaintedArmyVotes())));
        m_Ui->FullResultsTable->setItem(rowCount, 6, new QTableWidgetItem(IntToQString(player.GetMostSportingVotes())));
    }
}

void MainWindow::ResetPage()
{
    m_Ui->FirstPlaceInputLabel->clear();
    m_Ui->SecondPlaceInputLabel->clear();
    m_Ui->ThirdPlaceInputLabel->clear();
    m_Ui->WoodenSpoonInputLabel->clear();
    m_Ui->MostSportingInputLabel->clear();
    m_Ui->BestPaintedInputLabel->clear();

    m_Ui->BestPaintedInputLabel->setText("Not Set");
    m_Ui->BestPaintedVotedComboBox->setCurrentIndex(0);
    m_Ui->BestPaintedVotingComboBox->setCurrentIndex(0);

    m_Ui->MostSportingInputLabel->setText("Not Set");
    m_Ui->MostSportingVotedComboBox->setCurrentIndex(0);
    m_Ui->MostSportingVotingComboBox->setCurrentIndex(0);

    m_Ui->FirstPlaceInputLabel->setText("Not Set");
    m_Ui->SecondPlaceInputLabel->setText("Not Set");
    m_Ui->ThirdPlaceInputLabel->setText("Not Set");
    m_Ui->WoodenSpoonInputLabel->setText("Not Set");

    m_Ui->FullResultsTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Position"));
    m_Ui->FullResultsTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Player Name"));
    m_Ui->FullResultsTable->setHorizontalHeaderItem(2, new QTableWidgetItem("TPs"));
    m_Ui->FullResultsTable->setHorizontalHeaderItem(3, new QTableWidgetItem("VPs"));
    m_Ui->FullResultsTable->setHorizontalHeaderItem(4, new QTableWidgetItem("VP Difference"));
    m_Ui->FullResultsTable->setHorizontalHeaderItem(5, new QTableWidgetItem("Best Painted"));
    m_Ui->FullResultsTable->setHorizontalHeaderItem(6, new QTableWidgetItem("Most Sporting"));
}
