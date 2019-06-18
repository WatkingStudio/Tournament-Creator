#include "include/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::loadResultsPage()
{
    ui->stackedWidget->setCurrentIndex(Pages::RESULTS_PAGE);

    resetPage();
}

void MainWindow::on_bestPaintedPushButton_clicked()
{
    //Get Player Names
    std::string votingPlayer = ui->bestPaintedVotingLineEdit->text().toStdString();
    std::string votedPlayer = ui->bestPaintedVotedLineEdit->text().toStdString();
    bool addVote = false;
    //Check if Voting Player has Voted
    for(int i = 0; i < m_MainPlayerList.size(); ++i)
    {
        if(m_MainPlayerList.at(i).getName() == votingPlayer)
        {
            if(m_MainPlayerList.at(i).hasVotedBestPainted())
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Previously Voted", "This player has already voted. Do you wish to replace their previous vote?", QMessageBox::Yes|QMessageBox::No);
                if(reply == QMessageBox::Yes)
                {
                    for(int j = 0; j < m_MainPlayerList.size(); ++j)
                    {
                        if(m_MainPlayerList.at(j).getName() == m_MainPlayerList.at(i).getBestPaintedVote())
                        {
                            m_MainPlayerList.at(j).removeBestPaintedArmyVote();
                            break;
                        }
                    }
                    m_MainPlayerList.at(i).setBestPaintedVote(votedPlayer);
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
                m_MainPlayerList.at(i).setVotedBestPainted(true);
                m_MainPlayerList.at(i).setBestPaintedVote(votedPlayer);
                break;
            }

        }
    }

    if(addVote)
    {
        for(int i = 0; i < m_MainPlayerList.size(); ++i)
        {
            if(m_MainPlayerList.at(i).getName() == votedPlayer)
            {
                m_MainPlayerList.at(i).addBestPaintedArmyVote(m_BestPaintedValue);
                m_NumBestPaintedVotes++;
                break;
            }
        }
    }

    //clear field
    ui->bestPaintedVotedLineEdit->clear();
    ui->bestPaintedVotingLineEdit->clear();
}

void MainWindow::on_mostSportingPushButton_clicked()
{
    //Get Player Names
    std::string votingPlayer = ui->mostSportingVotingLineEdit->text().toStdString();
    std::string votedPlayer = ui->mostSportingVotedLineEdit->text().toStdString();
    bool addVote = false;

    //Check if Voting Player has Voted
    for(int i = 0; i < m_MainPlayerList.size(); ++i)
    {
        if(m_MainPlayerList.at(i).getName() == votingPlayer)
        {
            if(m_MainPlayerList.at(i).hasVotedBestPainted())
            {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Previously Voted", "This player has already voted. Do you wish to replace their previous vote?", QMessageBox::Yes|QMessageBox::No);
                if(reply == QMessageBox::Yes)
                {
                    for(int j = 0; j < m_MainPlayerList.size(); ++j)
                    {
                        if(m_MainPlayerList.at(j).getName() == m_MainPlayerList.at(i).getMostSportingVote())
                        {
                            m_MainPlayerList.at(j).removeMostSportingVote();
                            break;
                        }
                    }
                    m_MainPlayerList.at(i).setMostSportingVote(votedPlayer);
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
            m_MainPlayerList.at(i).setVotedMostSporting(true);
            m_MainPlayerList.at(i).setMostSportingVote(votedPlayer);
            break;
        }
    }

    if(addVote)
    {
        for(int i = 0; i < m_MainPlayerList.size(); ++i)
        {
            if(m_MainPlayerList.at(i).getName() == votedPlayer)
            {
                m_MainPlayerList.at(i).addMostSportingVote(m_MostSportingValue);
                m_NumMostSportingVotes++;
                break;
            }
        }
    }

    //clear field
    ui->mostSportingVotingLineEdit->clear();
    ui->mostSportingVotedLineEdit->clear();
}

void MainWindow::on_displayResultsPushButton_clicked()
{
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
        resetPage();
        updateRankings();

        //Fill in all award data
        std::string mostSportingPlayer = getMostSportingPlayer();
        std::string bestPaintedPlayer = getBestPaintedPlayer();
        std::string woodenSpoonPlayer = getWoodenSpoonPlayer();
        std::vector<std::string> podiumPlayers = getPodiumPlayers();

        ui->mostSportingInputLabel->setText(mostSportingPlayer.c_str());
        ui->bestPaintedInputLabel->setText(bestPaintedPlayer.c_str());
        ui->woodenSpoonInputLabel->setText(woodenSpoonPlayer.c_str());
        ui->firstPlaceInputLabel->setText(podiumPlayers.at(0).c_str());
        ui->secondPlaceInputLabel->setText(podiumPlayers.at(1).c_str());
        ui->thirdPlaceInputLabel->setText(podiumPlayers.at(2).c_str());

        populateResultsTable();
    }
}

std::string MainWindow::getMostSportingPlayer()
{
    std::string mostSportingPlayer;
    int mostSportingVotes = 0;

    for(auto player : m_MainPlayerList)
    {
        if(player.getMostSportingVotes() > mostSportingVotes)
        {
            mostSportingVotes = player.getMostSportingVotes();
            mostSportingPlayer = player.getName();
        }
    }

    return mostSportingPlayer;
}

std::string MainWindow::getBestPaintedPlayer()
{
    std::string bestPaintedPlayer;
    int bestPaintedVotes = 0;

    for(auto player : m_MainPlayerList)
    {
        if(player.getBestPaintedArmyVotes() > bestPaintedVotes)
        {
            bestPaintedVotes = player.getBestPaintedArmyVotes();
            bestPaintedPlayer = player.getName();
        }
    }

    return bestPaintedPlayer;
}

std::string MainWindow::getWoodenSpoonPlayer()
{
    return m_MainPlayerList.at(m_MainPlayerList.size() - 1).getName();
}

std::vector<std::string> MainWindow::getPodiumPlayers()
{
    std::vector<std::string> podiumPlayers;
    podiumPlayers.push_back(m_MainPlayerList.at(0).getName());
    podiumPlayers.push_back(m_MainPlayerList.at(1).getName());
    podiumPlayers.push_back(m_MainPlayerList.at(2).getName());

    return podiumPlayers;
}

void MainWindow::populateResultsTable()
{
    for(auto player : m_MainPlayerList)
    {
        int rowCount = ui->fullResultsTable->rowCount();
        ui->fullResultsTable->insertRow(rowCount);
        ui->fullResultsTable->setItem(rowCount, 0, new QTableWidgetItem(intToQString(rowCount + 1)));
        ui->fullResultsTable->setItem(rowCount, 1, new QTableWidgetItem(player.getName().c_str()));
        ui->fullResultsTable->setItem(rowCount, 2, new QTableWidgetItem(intToQString(player.getTPs())));
        ui->fullResultsTable->setItem(rowCount, 3, new QTableWidgetItem(intToQString(player.getVPs())));
        ui->fullResultsTable->setItem(rowCount, 4, new QTableWidgetItem(intToQString(player.getVPDiff())));
        ui->fullResultsTable->setItem(rowCount, 5, new QTableWidgetItem(intToQString(player.getBestPaintedArmyVotes())));
        ui->fullResultsTable->setItem(rowCount, 6, new QTableWidgetItem(intToQString(player.getMostSportingVotes())));
    }
}

void MainWindow::resetPage()
{
    ui->firstPlaceInputLabel->clear();
    ui->secondPlaceInputLabel->clear();
    ui->thirdPlaceInputLabel->clear();
    ui->woodenSpoonInputLabel->clear();
    ui->mostSportingInputLabel->clear();
    ui->bestPaintedInputLabel->clear();

    ui->bestPaintedInputLabel->setText("Not Set");
    ui->bestPaintedVotedLineEdit->clear();
    ui->bestPaintedVotingLineEdit->clear();

    ui->mostSportingInputLabel->setText("Not Set");
    ui->mostSportingVotedLineEdit->clear();
    ui->mostSportingVotingLineEdit->clear();

    ui->firstPlaceInputLabel->setText("Not Set");
    ui->secondPlaceInputLabel->setText("Not Set");
    ui->thirdPlaceInputLabel->setText("Not Set");
    ui->woodenSpoonInputLabel->setText("Not Set");

    ui->fullResultsTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Position"));
    ui->fullResultsTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Player Name"));
    ui->fullResultsTable->setHorizontalHeaderItem(2, new QTableWidgetItem("TPs"));
    ui->fullResultsTable->setHorizontalHeaderItem(3, new QTableWidgetItem("VPs"));
    ui->fullResultsTable->setHorizontalHeaderItem(4, new QTableWidgetItem("VP Difference"));
    ui->fullResultsTable->setHorizontalHeaderItem(5, new QTableWidgetItem("Best Painted"));
    ui->fullResultsTable->setHorizontalHeaderItem(6, new QTableWidgetItem("Most Sporting"));
}
