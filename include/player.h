#ifndef INCLUDEPLAYER_H
#define INCLUDEPLAYER_H

#include "util.h"
#include <string>
#include <QJsonObject>

class Player
{
public:
    Player() = default;
    Player(const Player &player);

    // Add a Best Painted Army vote.
    void AddBestPaintedArmyVote(const int value = 0);

    // Add a Draw result.
    void AddDraw(int value);

    // Add a Loss result.
    void AddLoss(int value);

    // Add a Most Sporting vote.
    void AddMostSportingVote(const int value = 0);

    // Add a Game result.
    void AddResult(const std::string &result, const int value);

    // Add Tournament Points.
    void AddTPs(const int points);

    // Add Victory Points Difference.
    void AddVPDiff(const int points);

    // Add Victory Points.
    void AddVPs(const int points);

    // Add a Win result.
    void AddWin(int value);


    // Who did this player vote for, for Best Painted.
    // @return The name of the player who was voted for.
    std::string GetBestPaintedVote() const;

    // How many Best Painted votes this player has.
    // @return The amount of Best Painted votes this player has.
    int GetBestPaintedArmyVotes() const;

    // How many Draws this player has.
    // @return The number of Draws this player has.
    int GetDraws() const;

    // How many Losses this player has.
    // @return The number of Losses this player has.
    int GetLosses() const;

    // Who did this player vote for, for Most Sporting
    // @return The name of the player who was voted for.
    std::string GetMostSportingVote() const;

    // How many Most Sporting votes this player has.
    // @return The amount of Most Sporting votes this player has.
    int GetMostSportingVotes() const;

    // Get this players Name.
    // @return The Name of this player.
    std::string GetName() const;

    // Gets the Player Data and constructs it into a QJson Object.
    // @return The Players Data as a QJsonObject.
    QJsonObject GetPlayerData() const;

    // Gets this players Seed.
    // @return The Seed of this player.
    int GetSeed() const;

    // Gets this players Tournament Points.
    // @return The amount of Tournament Points this player has.
    int GetTPs() const;

    // Gets this players Victory Point Difference.
    // @return The Victory Point Difference of this player.
    int GetVPDiff() const;

    // Gets this players Victory Points.
    // @return The amount of Victory Points this player has.
    int GetVPs() const;

    // Gets the amount of Wins this player has.
    // @return The amount of Wins this player has.
    int GetWins() const;


    // The overload of the = operator.
    void operator=(const Player &player);


    // Checks if this player has voted for Best Painted.
    // @return True if the player has voted, False if not.
    bool HasVotedBestPainted() const;

    // Checks if this player has voted for Most Sporting.
    // @return True if the player has voted, False if not.
    bool HasVotedMostSporting() const;

    // Check if this player has had their First Round Matchup set.
    // @return True if the it has been set, False if not.
    bool IsFirstRoundSet() const;


    // Removes a Best Painted Army vote.
    void RemoveBestPaintedArmyVote(const int value = 0);

    // Removes a Most Sporting vote.
    void RemoveMostSportingVote(const int value = 0);


    // Set which player this player voted for, for Best Painted.
    void SetBestPaintedVote(const std::string &name);

    // Set that this player has their First Round Matchup set.
    void SetFirstRoundSet(const bool set);

    // Set which player this player voted for, for Most Sporting
    void SetMostSportingVote(const std::string &name);

    // Set this players Name.
    void SetName(const std::string &name);

    // Set this Players Data from a QJsonObject.
    void SetPlayerData(const QJsonObject &object);

    // Set this players Seed.
    void SetSeed(const int seed);

    // Set if this player has voted for Best Painted.
    void SetVotedBestPainted(const bool set);

    // Set if this player has voted for Most Sporting.
    void SetVotedMostSporting(const bool set);
private:
    std::unique_ptr<std::string> m_Name = std::make_unique<std::string>("");
    std::unique_ptr<int> m_NumberDraws = std::make_unique<int>(0);
    std::unique_ptr<int> m_NumberLosses = std::make_unique<int>(0);
    std::unique_ptr<int> m_NumberWins = std::make_unique<int>(0);
    std::unique_ptr<int> m_Seed = std::make_unique<int>(-1);

    std::unique_ptr<int> m_TPs = std::make_unique<int>(0);
    std::unique_ptr<int> m_VPDiff = std::make_unique<int>(0);
    std::unique_ptr<int> m_VPs = std::make_unique<int>(0);

    std::unique_ptr<std::string> m_BestPaintedVote = std::make_unique<std::string>("");
    std::unique_ptr<int> m_BestPaintedArmyVotes = std::make_unique<int>(0);
    std::unique_ptr<std::string> m_MostSportingVote = std::make_unique<std::string>("");
    std::unique_ptr<int> m_MostSportingVotes = std::make_unique<int>(0);
    std::unique_ptr<bool> m_VotedBestPainted = std::make_unique<bool>(false);
    std::unique_ptr<bool> m_VotedMostSporting = std::make_unique<bool>(false);

    std::unique_ptr<bool> m_FirstRoundSet = std::make_unique<bool>(false);


private: //Json Tags
    const std::unique_ptr<std::string> m_NameTag = std::make_unique<std::string>("name");
    const std::unique_ptr<std::string> m_DrawsTag = std::make_unique<std::string>("draws");
    const std::unique_ptr<std::string> m_LossesTag = std::make_unique<std::string>("losses");
    const std::unique_ptr<std::string> m_WinsTag = std::make_unique<std::string>("wins");
    const std::unique_ptr<std::string> m_SeedTag = std::make_unique<std::string>("seed");

    const std::unique_ptr<std::string> m_TPsTag = std::make_unique<std::string>("tps");
    const std::unique_ptr<std::string> m_VPDiffTag = std::make_unique<std::string>("vp_diff");
    const std::unique_ptr<std::string> m_VPsTag = std::make_unique<std::string>("vps");

    const std::unique_ptr<std::string> m_BestPaintedVoteTag = std::make_unique<std::string>("best_painted_vote");
    const std::unique_ptr<std::string> m_BestPaintedTag = std::make_unique<std::string>("best_painted");
    const std::unique_ptr<std::string> m_MostSportingTag = std::make_unique<std::string>("most_sporting");
    const std::unique_ptr<std::string> m_MostSportingVoteTag = std::make_unique<std::string>("most_sporting_vote");
    const std::unique_ptr<std::string> m_VotedBestPaintedTag = std::make_unique<std::string>("voted_best_painted");
    const std::unique_ptr<std::string> m_VotedMostSportingTag = std::make_unique<std::string>("voted_most_sporting");

    const std::unique_ptr<std::string> m_FirstRoundSetTag = std::make_unique<std::string>("first_round_set");
};

#endif // INCLUDEPLAYER_H
