#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_Ui(new Ui::MainWindow)
{
    m_Ui->setupUi(this);

    connect(m_Ui->tournamentCreatorPlayerTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(playerEntrySelected(int, int)));
    connect(m_Ui->MatchupsPlayerTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(MatchupSelected(int,int)));
    connect(&m_MatchUpSwapWidget, SIGNAL(SwapComplete(const Player &, const Player &, const Player &, const Player &)), this, SLOT(NewMatchUpsFromSwap(const Player &, const Player &, const Player &, const Player &)));
    connect(&m_EventSettingsWidget, SIGNAL(SettingsComplete(EventSettingsData)), this, SLOT(ReceiveEventSettings(EventSettingsData)));

    InitialiseResultsComboBox(m_Ui->MatchupsPlayerOneComboBox);
    InitialiseResultsComboBox(m_Ui->MatchupsPlayerTwoComboBox);

    LoadStartPage();
}

MainWindow::~MainWindow()
{
    delete m_Ui;
}

void MainWindow::playerEntrySelected(int row, int col)
{
    m_TournamentCreatorSelectedRow = row;
    m_TournamentCreatorSelectedCol = col;
}

void MainWindow::MatchupSelected(int row, int col)
{
    *m_MatchupsSelectedRow = row;
    *m_MatchupsSelectedCol = col;

    m_Ui->MatchupsModifyMatchupsButton->setEnabled(true);

    UpdateRoundInput();
}

void MainWindow::InitialiseResultsComboBox(QComboBox* comboBox)
{
    comboBox->addItem("");
    comboBox->addItem("Win");
    comboBox->addItem("Draw");
    comboBox->addItem("Loss");
}

bool MainWindow::LoadEventData()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Event Data"), "", tr("Json (*.json)"));
    //Read the data from the .json file
    if(fileName == "")
        return false;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonData = file.readAll();
    file.close();

    //Convert the .json data into a format we can use
    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonObject object = document.object();

    //Event Settings
    {
        m_NumberOfRounds = object[m_NumRoundsTag.c_str()].toInt();
        m_CurrentRoundNumber = object[m_CurrentRoundTag.c_str()].toInt();
        m_ActiveRoundNumber = object[m_ActiveRoundTag.c_str()].toInt();
        m_UsingSeed = object[m_UsingSeedTag.c_str()].toBool();
        *m_WinValue = object[m_WinValueTag.c_str()].toInt();
        *m_DrawValue = object[m_DrawValueTag.c_str()].toInt();
        *m_LossValue = object[m_LossValueTag.c_str()].toInt();
        m_MostSportingValue = object[m_MostSportingValueTag.c_str()].toInt();
        m_BestPaintedValue = object[m_BestPaintedValueTag.c_str()].toInt();
    }

    //Tiebreaker Settings
    {
        m_FirstTiebreaker = Tiebreak::StringToTiebreaker(object[m_FirstTiebreakerTag.c_str()].toString().toStdString());
        m_SecondTiebreaker = Tiebreak::StringToTiebreaker(object[m_SecondTiebreakerTag.c_str()].toString().toStdString());
        m_ThirdTiebreaker = Tiebreak::StringToTiebreaker(object[m_ThirdTiebreakerTag.c_str()].toString().toStdString());
        m_FourthTiebreaker = Tiebreak::StringToTiebreaker(object[m_FourthTiebreakerTag.c_str()].toString().toStdString());
    }

    m_EventDataFileName = object[m_EventDataFileNameTag.c_str()].toString().toStdString();

    //Get array of player data
    QJsonValue playerValue = object.value(m_PlayerDataTag.c_str());
    QJsonArray playerArray = playerValue.toArray();

    //Player Data
    for(int i = 0; i < playerArray.count(); i++)
    {
        //Convert each element of the array into an object and use it to set player data
        QJsonObject item = playerArray.at(i).toObject();
        Player newPlayer;
        newPlayer.SetPlayerData(item);
        m_MainPlayerList.push_back(newPlayer);
    }

    QJsonValue allMatchupsValue = object.value(m_AllMatchupsTag.c_str());
    QJsonArray allMatchupsArray = allMatchupsValue.toArray();

    //Matchup Data
    for(int i = 0; i < allMatchupsArray.count(); i++)
    {
        QJsonArray singleMatchupArray = allMatchupsArray.at(i).toArray();
        std::vector<std::pair<Player, Player>> tempRoundMatchups;
        for(int j = 0; j < singleMatchupArray.count(); j++)
        {
            QJsonObject item = singleMatchupArray.at(j).toObject();
            std::pair<Player, Player> tempMatchup;
            tempMatchup.first.SetPlayerData(item[m_PairFirstTag.c_str()].toObject());
            tempMatchup.second.SetPlayerData(item[m_PairSecondTag.c_str()].toObject());

            tempRoundMatchups.push_back(tempMatchup);
        }
        m_AllRoundMatchups->push_back(tempRoundMatchups);
    }

    UtilLog("Event Data Loaded");
    return true;
}

void MainWindow::SaveEventData() const
{
    QJsonObject jsonObject; //This will be saved to the JsonDocument
    QJsonArray playerArray; //This holds the json data for the players

    //Event Settings
    {
        jsonObject[m_NumRoundsTag.c_str()] = m_NumberOfRounds;
        jsonObject[m_CurrentRoundTag.c_str()] = m_CurrentRoundNumber;
        jsonObject[m_ActiveRoundTag.c_str()] = m_ActiveRoundNumber;
        jsonObject[m_UsingSeedTag.c_str()] = m_UsingSeed;
        jsonObject[m_WinValueTag.c_str()] = *m_WinValue;
        jsonObject[m_DrawValueTag.c_str()] = *m_DrawValue;
        jsonObject[m_LossValueTag.c_str()] = *m_LossValue;
        jsonObject[m_MostSportingValueTag.c_str()] = m_MostSportingValue;
        jsonObject[m_BestPaintedValueTag.c_str()] = m_BestPaintedValue;
    }

    //Tiebreaker Settings
    {
        jsonObject[m_FirstTiebreakerTag.c_str()] = Tiebreak::TiebreakerToString(m_FirstTiebreaker).c_str();
        jsonObject[m_SecondTiebreakerTag.c_str()] = Tiebreak::TiebreakerToString(m_SecondTiebreaker).c_str();
        jsonObject[m_ThirdTiebreakerTag.c_str()] = Tiebreak::TiebreakerToString(m_ThirdTiebreaker).c_str();
        jsonObject[m_FourthTiebreakerTag.c_str()] = Tiebreak::TiebreakerToString(m_FourthTiebreaker).c_str();
    }

    jsonObject[m_EventDataFileNameTag.c_str()] = m_EventDataFileName.c_str();

    //Player Data
    for(const auto &player : m_MainPlayerList)
    {
        playerArray.append(player.GetPlayerData());
    }

    jsonObject[m_PlayerDataTag.c_str()] = playerArray;

    //Matchup Data
    {
        QJsonArray currentMatchupsArray;
        //Current Round Data
        for(const auto &matchup : *m_CurrentRoundMatchups)
        {
            QJsonObject tempMatchup;
            tempMatchup[m_PairFirstTag.c_str()] = matchup.first.GetPlayerData();
            tempMatchup[m_PairSecondTag.c_str()] = matchup.second.GetPlayerData();
            currentMatchupsArray.append(tempMatchup);
        }
        jsonObject[m_CurrentMatchupsTag.c_str()] = currentMatchupsArray;

        QJsonArray allMatchupsArray;
        //All Round Data
        for(const auto &round : *m_AllRoundMatchups)
        {
            QJsonArray matchupsArray;
            for(const auto &matchup : round)
            {
                QJsonObject tempMatchup;
                tempMatchup[m_PairFirstTag.c_str()] = matchup.first.GetPlayerData();
                tempMatchup[m_PairSecondTag.c_str()] = matchup.second.GetPlayerData();
                matchupsArray.append(tempMatchup);
            }
            allMatchupsArray.append(matchupsArray);
        }
        jsonObject[m_AllMatchupsTag.c_str()] = allMatchupsArray;
    }

    //Convert data into a document so we can save it to file
    QJsonDocument doc(jsonObject);
    QFile file(m_EventDataFileName.c_str());
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(doc.toJson());
        file.close();
    }
    else
    {
        UtilLog("ERROR: Unable to open file " + file.fileName().toStdString());
        UtilLog("Attempting to create another file");
        CreateFile(m_EventDataFileName.c_str(), *m_EventDirectory);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.write(doc.toJson());
            file.close();
        }
        else
        {
            UtilLog("Failed to create file");
            QMessageBox::warning(0, "Saving Event Data Error", "Failed to create save data file.");
        }
    }

    UtilLog("Event Data Saved");
}
