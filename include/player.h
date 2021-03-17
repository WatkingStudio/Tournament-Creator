#ifndef INCLUDEPLAYER_H
#define INCLUDEPLAYER_H

#include "util.h"
#include <string>
#include <QJsonObject>

class Player
{
public:
    Player() = default;

    std::string getName() const;
    int getSeed() const;
    int getMostSportingVotes() const;
    int getBestPaintedArmyVotes() const;

    int getWins() const;
    int getDraws() const;
    int getLosses() const;
    int getTPs() const;
    int getVPs() const;
    int getVPDiff() const;

    void addWin(int value);
    void addDraw(int value);
    void addLoss(int value);

    bool isFirstRoundSet() const;
    bool hasVotedBestPainted() const;
    bool hasVotedMostSporting() const;
    std::string getBestPaintedVote() const;
    std::string getMostSportingVote() const;

    void setName(const std::string &name);
    void setSeed(int seed);

    void addMostSportingVote(int value = 0);
    void addBestPaintedArmyVote(int value = 0);
    void removeMostSportingVote(int value = 0);
    void removeBestPaintedArmyVote(int value = 0);

    void addTPs(int points);
    void addVPs(int points);
    void addVPDiff(int points);
    void addResult(const std::string &result, int value);

    void setFirstRoundSet(bool set);
    void setVotedBestPainted(bool set);
    void setVotedMostSporting(bool set);
    void setBestPaintedVote(const std::string &name);
    void setMostSportingVote(const std::string &name);

    QJsonObject getPlayerData() const;
    void setPlayerData(QJsonObject object);

private:
    std::string m_Name = "";
    int m_Seed = -1;
    int m_MostSportingVotes = 0;
    int m_BestPaintedArmyVotes = 0;

    int m_NumberWins = 0;
    int m_NumberDraws = 0;
    int m_NumberLosses = 0;
    int m_VPs = 0;
    int m_TPs = 0;
    int m_VPDiff = 0;

    bool m_FirstRoundSet{false};
    bool m_VotedBestPainted{false};
    std::string m_BestPaintedVote = "";
    bool m_VotedMostSporting{false};
    std::string m_MostSportingVote = "";

private: //Json Tags
    std::string m_NameTag = "name";
    std::string m_SeedTag = "seed";
    std::string m_MostSportingTag = "most_sporting";
    std::string m_BestPaintedTag = "best_painted";
    std::string m_WinsTag = "wins";
    std::string m_DrawsTag = "draws";
    std::string m_LossesTag = "losses";
    std::string m_VPsTag = "vps";
    std::string m_TPsTag = "tps";
    std::string m_VPDiffTag = "vp_diff";
    std::string m_FirstRoundSetTag = "first_round_set";
    std::string m_VotedBestPaintedTag = "voted_best_painted";
    std::string m_VotedMostSportingTag = "voted_most_sporting";
    std::string m_BestPaintedVoteTag = "best_painted_vote";
    std::string m_MostSportingVoteTag = "most_sporting_vote";
};

#endif // INCLUDEPLAYER_H
