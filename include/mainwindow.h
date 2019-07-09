#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "util.h"
#include "player.h"
#include <QMessageBox>
#include "directmatchupswap.h"

namespace Ui {
class MainWindow;
}

enum Pages{
    START_SCREEN,
    TOURNAMENT_CREATOR,
    MATCHUPS_PAGE,
    RESULTS_PAGE,
    LAST
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    DirectMatchupSwap m_MatchUpSwapWidget;

private slots:
    //Global
    void playerEntrySelected(int, int);

    //Start Page Slots
    void on_createNewEventButton_clicked();
    void on_loadEventButton_clicked();

    //Tournament Creator Slots
    void on_removePlayerButton_clicked();
    void on_addPlayerButton_clicked();
    void on_saveEventTournamentCreatorButton_clicked();
    void on_eventSettingsButton_clicked();
    void on_continueTournamentCreatorButton_clicked();
    void on_backTournamentCreatorButton_clicked();

    //Matchups Slots
    void on_matchupsBackButton_clicked();
    void matchupSelected(int, int);
    void on_matchupsEnterResultsButton_clicked();
    void on_matchupsModifyMatchupsButton_clicked();
    void on_matchupsNextRoundButton_clicked();
    void on_matchupsResetMatchupTable_clicked();
    void on_matchupsDirectMatchupSwapButton_clicked();
    void newMatchUpsFromSwap(const Player &playerOne, const Player &playerTwo, const Player &playerThree, const Player &playerFour);

    //Result Page Slots
    void on_bestPaintedPushButton_clicked();
    void on_mostSportingPushButton_clicked();
    void on_displayResultsPushButton_clicked();

    void on_usingSeededPlayersCheckBox_stateChanged(int arg1);

private:
    //General Functions and Variables
    Ui::MainWindow *ui;
    bool m_DebugMessagesOn{true};
    std::string m_EventDataFileName = "eventData.json";

    std::vector<Player> m_MainPlayerList;

    int m_NumberOfRounds = 6;
    int m_CurrentRoundNumber = 0;
    int m_ActiveRoundNumber = 0;
    bool m_UsingSeed{false};
    int m_NumBestPaintedVotes = 0;
    int m_NumMostSportingVotes = 0;

    void saveEventData();
    void loadEventData();

    //Start Page Functions and Variables
    void loadStartPage();

    //Tournament Creator Page Functions and Variables
    void loadTournamentCreatorPage();
    void updatePlayerTable();
    void resetPlayerTable();

    std::vector<Player> m_TempPlayerList;
    int m_TournamentCreatorSelectedRow = -1;
    int m_TournamentCreatorSelectedCol = -1;
    bool m_EventSettingsActive = false;

    //Matchups Page Functions and Variables
    void loadMatchupsPage();
    void loadMatchupsPageFromLoadedEvent();
    void setLoadedEventMatchups();
    void setInitialMatchups();
    void createSeededMatchup();
    void createRandomMatchup();
    void updateScores();
    void createRankedMatchup();
    void updateRankings();
    void resetMatchupsTable();
    void updateMatchupsTable();
    void resetRoundInput();
    void disableRoundInput();
    void enableRoundInput();
    void updateRoundInput();
    bool isRoundFinished() const;
    bool resultsValid() const;
    void addResult(const std::string &result, int playerIndex);
    void setMatchUp(const Player &playerOne, const Player &playerTwo);
    Player findRandomPlayer(int playerCount);

    std::vector<std::vector<std::pair<Player, Player>>> m_AllRoundMatchups;
    std::vector<std::pair<Player,Player>> m_CurrentRoundMatchups;
    int m_MatchupsSelectedRow = -1;
    int m_MatchupsSelectedCol = -1;
    bool m_TableIsVictory{false};
    int m_WinValue = 3;
    int m_DrawValue = 1;
    int m_LossValue = 0;

    //Results Page Functions and Vartiables
    void loadResultsPage();
    std::string getMostSportingPlayer();
    std::string getBestPaintedPlayer();
    std::string getWoodenSpoonPlayer();
    std::vector<std::string> getPodiumPlayers();
    void populateResultsTable();
    void resetPage();

    int m_MostSportingValue = 0;
    int m_BestPaintedValue = 0;
private: //Json Strings
    std::string m_NumRoundsTag = "number_of_rounds";
    std::string m_CurrentRoundTag = "current_round";
    std::string m_ActiveRoundTag = "active_round";
    std::string m_UsingSeedTag = "using_seed";
    std::string m_WinValueTag = "win_value";
    std::string m_DrawValueTag = "draw_value";
    std::string m_LossValueTag = "loss_value";
    std::string m_MostSportingValueTag = "most_sporting_value";
    std::string m_BestPaintedValueTag = "best_painted_value";
    std::string m_PlayerDataTag = "player_data";
    std::string m_PairFirstTag = "first";
    std::string m_PairSecondTag = "second";
    std::string m_CurrentMatchupsTag = "current_matchups";
    std::string m_AllMatchupsTag = "all_matchups";
};

#endif // MAINWINDOW_H
