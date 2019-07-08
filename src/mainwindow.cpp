#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setDebugging(m_DebugMessagesOn);

    //m_MatchUpSwapWidget = std::make_shared<DirectMatchupSwap>();

    connect(ui->tournamentCreatorPlayerTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(playerEntrySelected(int, int)));
    connect(ui->matchupsPlayerTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(matchupSelected(int,int)));
    connect(&m_MatchUpSwapWidget, SIGNAL(SwapComplete(Player, Player, Player, Player)), this, SLOT(newMatchUpsFromSwap(Player, Player, Player, Player)));

    ui->matchupsPlayerOneComboBox->addItem(QString(""));
    ui->matchupsPlayerOneComboBox->addItem(QString("Win"));
    ui->matchupsPlayerOneComboBox->addItem(QString("Draw"));
    ui->matchupsPlayerOneComboBox->addItem(QString("Loss"));

    ui->matchupsPlayerTwoComboBox->addItem(QString(""));
    ui->matchupsPlayerTwoComboBox->addItem(QString("Win"));
    ui->matchupsPlayerTwoComboBox->addItem(QString("Draw"));
    ui->matchupsPlayerTwoComboBox->addItem(QString("Loss"));

    loadStartPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playerEntrySelected(int row, int col)
{
    m_TournamentCreatorSelectedRow = row;
    m_TournamentCreatorSelectedCol = col;
}

void MainWindow::matchupSelected(int row, int col)
{
    m_MatchupsSelectedRow = row;
    m_MatchupsSelectedCol = col;

    ui->matchupsModifyMatchupsButton->setEnabled(true);

    updateRoundInput();
}

//Possible optimisation is to only save the name and seed of
// the player, and then on load search for the player in the
// player list and copy over. This will reduce the size of
// the .json file.
void MainWindow::saveEventData()
{
    QJsonObject jsonObject; //This will be saved to the JsonDocument
    QJsonArray playerArray; //This holds the json data for the players

    //Event Settings
    {
        jsonObject[m_NumRoundsTag.c_str()] = m_NumberOfRounds;
        jsonObject[m_CurrentRoundTag.c_str()] = m_CurrentRoundNumber;
        jsonObject[m_ActiveRoundTag.c_str()] = m_ActiveRoundNumber;
        jsonObject[m_UsingSeedTag.c_str()] = m_UsingSeed;
        jsonObject[m_WinValueTag.c_str()] = m_WinValue;
        jsonObject[m_DrawValueTag.c_str()] = m_DrawValue;
        jsonObject[m_LossValueTag.c_str()] = m_LossValue;
        jsonObject[m_MostSportingValueTag.c_str()] = m_MostSportingValue;
        jsonObject[m_BestPaintedValueTag.c_str()] = m_BestPaintedValue;
    }

    //Player Data
    for(auto player : m_MainPlayerList)
    {
        QJsonObject object;
        object = player.getPlayerData();

        playerArray.append(object);
    }

    jsonObject[m_PlayerDataTag.c_str()] = playerArray;

    //Matchup Data
    {
        QJsonArray currentMatchupsArray;
        //Current Round Data
        for(auto matchup : m_CurrentRoundMatchups)
        {
            QJsonObject object;
            object[m_PairFirstTag.c_str()] = matchup.first.getPlayerData();
            object[m_PairSecondTag.c_str()] = matchup.second.getPlayerData();
            currentMatchupsArray.append(object);
        }
        jsonObject[m_CurrentMatchupsTag.c_str()] = currentMatchupsArray;

        QJsonArray allMatchupsArray;
        //All Round Data
        for(auto round : m_AllRoundMatchups)
        {
            QJsonArray matchupsArray;
            for(auto matchup : round)
            {
                QJsonObject object;
                object[m_PairFirstTag.c_str()] = matchup.first.getPlayerData();
                object[m_PairSecondTag.c_str()] = matchup.second.getPlayerData();
                matchupsArray.append(object);
            }
            allMatchupsArray.append(matchupsArray);
        }
        jsonObject[m_AllMatchupsTag.c_str()] = allMatchupsArray;
    }

    //Convert data into a document so we can save it to file
    QJsonDocument doc(jsonObject);
    QFile file(m_EventDataFileName.c_str());
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());

    utilLog("Event Data Saved");
}

void MainWindow::loadEventData()
{
    //Read the data from the .json file
    QFile file(m_EventDataFileName.c_str());
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
        m_WinValue = object[m_WinValueTag.c_str()].toInt();
        m_DrawValue = object[m_DrawValueTag.c_str()].toInt();
        m_LossValue = object[m_LossValueTag.c_str()].toInt();
        m_MostSportingValue = object[m_MostSportingValueTag.c_str()].toInt();
        m_BestPaintedValue = object[m_BestPaintedValueTag.c_str()].toInt();
    }

    //Get array of player data
    QJsonValue playerValue = object.value(m_PlayerDataTag.c_str());
    QJsonArray playerArray = playerValue.toArray();

    //Player Data
    for(int i = 0; i < playerArray.count(); i++)
    {
        //Convert each element of the array into an object and use it to set player data
        QJsonObject item = playerArray.at(i).toObject();
        Player newPlayer;
        newPlayer.setPlayerData(item);
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
            tempMatchup.first.setPlayerData(item[m_PairFirstTag.c_str()].toObject());
            tempMatchup.second.setPlayerData(item[m_PairSecondTag.c_str()].toObject());

            tempRoundMatchups.push_back(tempMatchup);
        }
        m_AllRoundMatchups.push_back(tempRoundMatchups);
    }

    utilLog("Event Data Loaded");
}
