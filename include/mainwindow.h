#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "util.h"
#include "player.h"
#include <QMessageBox>
#include "directmatchupswap.h"
#include "eventsettings.h"
#include <QStandardPaths>
#include <QFileDialog>
#include <QComboBox>
#include <memory>

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

enum TiebreakerResult{
    HIGHER,
    EQUAL,
    LOWER
};

struct PodiumPlayers{
    std::string PlayerOne;
    std::string PlayerTwo;
    std::string PlayerThree;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    DirectMatchupSwap m_MatchUpSwapWidget;
    EventSettings m_EventSettingsWidget;

private slots:
    //Global
    // Sets which cell has been clicked on.
    void PlayerEntrySelected(int, int);

    //Start Page Slots
    // Creates a new event.
    void on_CreateNewEventButton_clicked();

    // Loads a previous event.
    void on_LoadEventButton_clicked();

    //Tournament Creator Slots
    // Add the new player.
    void on_AddPlayerButton_clicked();

    // Return to the previous page.
    void on_BackTournamentCreatorButton_clicked();

    // Continue to the matchups page.
    void on_ContinueTournamentCreatorButton_clicked();

    // Load the event settings widget.
    void on_EventSettingsButton_clicked();

    // Remove the selected player from the list.
    void on_RemovePlayerButton_clicked();

    // Save the current event data.
    void on_SaveEventTournamentCreatorButton_clicked();

    // Receive the event settings when they are set.
    void ReceiveEventSettings(const EventSettingsData &eventSettingsData);

    //Matchups Slots
    // When a matchup is selected in the MatchupTable handle it.
    void MatchupSelected(int row, int col);

    // A new matchup has been determined, modify the event data to observe the change.
    void NewMatchUpsFromSwap(const Player &playerOne, const Player &playerTwo, const Player &playerThree, const Player &playerFour);

    // Return to the Tournament Creator page.
    void on_MatchupsBackButton_clicked();

    // Clear the Matchups fields
    void on_MatchupsClearInputButton_clicked();

    // Perform a direct matchup swap between four players.
    void on_MatchupsDirectMatchupSwapButton_clicked();

    // Enter the results of the specified matchup.
    void on_MatchupsEnterResultsButton_clicked();

    // Modify the matchup with the least impact to the event.
    void on_MatchupsModifyMatchupsButton_clicked();

    // Take the event to the next round.
    void on_MatchupsNextRoundButton_clicked();

    // When PlayerOne's combo box index is changed.
    void on_MatchupsPlayerOneComboBox_currentIndexChanged(int index);

    // When PlayerTwo's combo box index is changed.
    void on_MatchupsPlayerTwoComboBox_currentIndexChanged(int index);

    // Reset the matchup table to what it was at the beginning of the round.
    void on_MatchupsResetMatchupTable_clicked();


    //Result Page Slots
    // Registers a Best Painted vote.
    void on_BestPaintedPushButton_clicked();

    // Displays the results of the event.
    void on_DisplayResultsPushButton_clicked();

    // Registers a Most Sporting vote.
    void on_MostSportingPushButton_clicked();

private:
    //General Functions and Variables
    Ui::MainWindow *m_Ui;

    std::unique_ptr<int> m_ActiveRoundNumber = std::make_unique<int>(0);
    std::unique_ptr<int> m_CurrentRoundNumber = std::make_unique<int>(0);
    std::unique_ptr<bool> m_DebugMessagesOn = std::make_unique<bool>(true);
    std::unique_ptr<std::string> m_EventDataFileName = std::make_unique<std::string>("eventData.json");
    std::unique_ptr<std::vector<Player>> m_MainPlayerList = std::make_unique<std::vector<Player>>();
    std::unique_ptr<uint> m_NumBestPaintedVotes = std::make_unique<uint>(0);
    std::unique_ptr<uint> m_NumMostSportingVotes = std::make_unique<uint>(0);
    std::unique_ptr<int> m_NumberOfRounds = std::make_unique<int>(6);
    std::unique_ptr<bool> m_UsingSeed = std::make_unique<bool>(false);

    // Initialises the passed combo box with the results strings.
    void InitialiseResultsComboBox(QComboBox* comboBox);

    // Loads the event data from a chosen .json file.
    // @return True if the load is successful, False if it is not.
    bool LoadEventData();

    // Saves the event data into a .json file tied to the event.
    void SaveEventData() const;

    //Start Page Functions and Variables
    // Loads the start page.
    void LoadStartPage();

    // Tournament Creator Page Functions and Variables
    // Loads the Tournament Creator Page.
    void LoadTournamentCreatorPage();

    // Updates the Player table to use the current Player list.
    void UpdatePlayerTable();

    // Resets the Player table.
    void ResetPlayerTable();

    std::unique_ptr<QString> m_EventDirectory = std::make_unique<QString>("TournamentData");
    std::unique_ptr<QString> m_EventDefaultFileName = std::make_unique<QString>("/EventData_");
    std::unique_ptr<std::vector<Player>> m_TempPlayerList = std::make_unique<std::vector<Player>>();
    std::unique_ptr<int> m_TournamentCreatorSelectedRow = std::make_unique<int>(-1);
    std::unique_ptr<int> m_TournamentCreatorSelectedCol = std::make_unique<int>(-1);
    std::unique_ptr<bool> m_EventSettingsActive = std::make_unique<bool>(false);
    std::unique_ptr<Tiebreak::Tiebreaker> m_FirstTiebreaker = std::make_unique<Tiebreak::Tiebreaker>(Tiebreak::Tiebreaker::VP_TOTAL);
    std::unique_ptr<Tiebreak::Tiebreaker> m_SecondTiebreaker = std::make_unique<Tiebreak::Tiebreaker>(Tiebreak::Tiebreaker::VP_DIFF);
    std::unique_ptr<Tiebreak::Tiebreaker> m_ThirdTiebreaker = std::make_unique<Tiebreak::Tiebreaker>(Tiebreak::Tiebreaker::MOST_SPORTING);
    std::unique_ptr<Tiebreak::Tiebreaker> m_FourthTiebreaker = std::make_unique<Tiebreak::Tiebreaker>(Tiebreak::Tiebreaker::BEST_PAINTED);

    //Matchups Page Functions and Variables//
    // Adds the passed result to the players stats.
    void AddResult(const std::string &result, const int playerIndex);

    // Checks the first tiebreaker between two players.
    // @return The result of the tiebreaker comparison.
    TiebreakerResult CheckFirstTiebreaker(const Player &playerOne, const Player &playerTwo) const;

    // Checks the second tiebreaker between two players.
    // @return The result of the tiebreaker comparison.
    TiebreakerResult CheckSecondTiebreaker(const Player &playerOne, const Player &playerTwo) const;

    // Checks the third tiebreaker between two players.
    // @return The result of the tiebreaker comparison.
    TiebreakerResult CheckThirdTiebreaker(const Player &playerOne, const Player &playerTwo) const;

    // Checks the fourth tiebreaker between two players.
    // @return The result of the tiebreaker comparison.
    TiebreakerResult CheckFourthTiebreaker(const Player &playerOne, const Player &playerTwo) const;

    // Checks the specified tiebreaker between two players.
    // @return The return to the tiebreaker comparison.
    TiebreakerResult CheckTiebreaker(const Tiebreak::Tiebreaker tiebreak, const Player &playerOne, const Player &playerTwo) const;

    // Checks the tiebreakers between two players, to see which should be ranking
    //     higher.
    // @return If player one beats player two on tiebreakers.
    bool CheckTiebreakers(const Player &playerOne, const Player &playerTwo) const;

    // Compares the two passed tiebreaker values.
    // @return If playerOne's value is higher, equal or lower than playerTwo's.
    TiebreakerResult CheckTiebreakerValue(const int playerOne, const int playerTwo) const;

    // Create the initial matchups for the event.
    void CreateInitialMatchups();

    // Create the inital matchup with random matchups.
    void CreateRandomMatchup();

    // Create matchups usings the ranking table.
    void CreateRankedMatchup();

    // Create the initial matchup using players seeds.
    void CreateSeededMatchup();

    // Disables the round input fields.
    void DisableRoundInput();

    // Enables the round input fields.
    void EnableRoundInput();

    // Finds a random player from the list.
    // @return The random player.
    Player FindRandomPlayer(const int playerCount);

    // Checks to see if all matchups have results.
    // @return True if all matchups have results, False if they do not.
    bool IsRoundFinished() const;

    // Load the matchups page.
    void LoadMatchupsPage();

    // Load the matchups page from a previous event.
    void LoadMatchupsPageFromLoadedEvent();

    // Reset the matchups table to the start of the round.
    void ResetMatchupsTable();

    // Resets the round input fields.
    void ResetRoundInput();

    // Checks to see if an input result is valid.
    // @return True if the input results are valid, False if they are not.
    bool ResultsValid() const;

    // Update the matchups to display the matchups.
    void UpdateMatchupsTable();

    // Update the player rankings.
    void UpdatePlayerRankingList();

    // Update the rankings.
    void UpdateRankings();

    // Updates the round input fields.
    void UpdateRoundInput();

    // Update the scores of all the players.
    void UpdateScores();

    // Updates the matchup table to have the new matchup.
    void UpdateTableMatchUp(const Player &playerOne, const Player &playerTwo);

    std::unique_ptr<std::vector<std::vector<std::pair<Player, Player>>>> m_AllRoundMatchups = std::make_unique<std::vector<std::vector<std::pair<Player, Player>>>>();
    std::unique_ptr<std::vector<std::pair<Player,Player>>> m_CurrentRoundMatchups = std::make_unique<std::vector<std::pair<Player, Player>>>();
    std::unique_ptr<int> m_MatchupsSelectedRow = std::make_unique<int>(-1);
    std::unique_ptr<int> m_MatchupsSelectedCol = std::make_unique<int>(-1);
    std::unique_ptr<bool> m_TableIsVictory = std::make_unique<bool>(false);
    std::unique_ptr<int> m_WinValue = std::make_unique<int>(3);
    std::unique_ptr<int> m_DrawValue = std::make_unique<int>(1);
    std::unique_ptr<int> m_LossValue = std::make_unique<int>(0);

    std::unique_ptr<bool> m_MatchUpDataBeingInput = std::make_unique<bool>(false);

    //Results Page Functions and Vartiables
    // Gets the player with the most Best Painted votes.
    // @return The name of the player who has the most Best Painted votes.
    std::string GetBestPaintedPlayer() const;

    // Gets the player with the most Most Sporting votes.
    // @return The name of the player who has the most Most Sporting votes.
    std::string GetMostSportingPlayer() const;

    // Gets the names of top three players.
    // @return The names of the top three players, in string form.
    PodiumPlayers GetPodiumPlayers() const;

    // Gets the player in the last position.
    // @return The name of the player in the last position.
    std::string GetWoodenSpoonPlayer() const;

    // Sets up the Results Page.
    void LoadResultsPage();

    // Populates the passed combo box.
    void PopulateComboBox(QComboBox &comboBox);

    // Populates the Results Table.
    void PopulateResultsTable();

    // Resets the results page.
    void ResetPage();

    std::unique_ptr<int> m_MostSportingValue = std::make_unique<int>(0);
    std::unique_ptr<int> m_BestPaintedValue = std::make_unique<int>(0);
private: //Json Strings
    std::unique_ptr<std::string> m_NumRoundsTag = std::make_unique<std::string>("number_of_rounds");
    std::unique_ptr<std::string> m_CurrentRoundTag = std::make_unique<std::string>("current_round");
    std::unique_ptr<std::string> m_ActiveRoundTag = std::make_unique<std::string>("active_round");
    std::unique_ptr<std::string> m_UsingSeedTag = std::make_unique<std::string>("using_seed");
    std::unique_ptr<std::string> m_WinValueTag = std::make_unique<std::string>("win_value");
    std::unique_ptr<std::string> m_DrawValueTag = std::make_unique<std::string>("draw_value");
    std::unique_ptr<std::string> m_LossValueTag = std::make_unique<std::string>("loss_value");
    std::unique_ptr<std::string> m_MostSportingValueTag = std::make_unique<std::string>("most_sporting_value");
    std::unique_ptr<std::string> m_BestPaintedValueTag = std::make_unique<std::string>("best_painted_value");
    std::unique_ptr<std::string> m_PlayerDataTag = std::make_unique<std::string>("player_data");
    std::unique_ptr<std::string> m_PairFirstTag = std::make_unique<std::string>("first");
    std::unique_ptr<std::string> m_PairSecondTag = std::make_unique<std::string>("second");
    std::unique_ptr<std::string> m_CurrentMatchupsTag = std::make_unique<std::string>("current_matchups");
    std::unique_ptr<std::string> m_AllMatchupsTag = std::make_unique<std::string>("all_matchups");
    std::unique_ptr<std::string> m_FirstTiebreakerTag = std::make_unique<std::string>("first_tiebreaker");
    std::unique_ptr<std::string> m_SecondTiebreakerTag = std::make_unique<std::string>("second_tiebreaker");
    std::unique_ptr<std::string> m_ThirdTiebreakerTag = std::make_unique<std::string>("third_tiebreaker");
    std::unique_ptr<std::string> m_FourthTiebreakerTag = std::make_unique<std::string>("fourth_tiebreaker");
    std::unique_ptr<std::string> m_EventDataFileNameTag = std::make_unique<std::string>("event_file");
};

#endif // MAINWINDOW_H
