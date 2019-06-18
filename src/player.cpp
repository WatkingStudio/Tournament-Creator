#include "include\player.h"
#include <string>

std::string Player::getName() const
{
    return m_Name;
}

int Player::getSeed() const
{
    return m_Seed;
}

int Player::getMostSportingVotes() const
{
    return m_MostSportingVotes;
}

int Player::getBestPaintedArmyVotes() const
{
    return m_BestPaintedArmyVotes;
}

int Player::getWins() const
{
    return m_NumberWins;
}

int Player::getDraws() const
{
    return m_NumberDraws;
}

int Player::getLosses() const
{
    return m_NumberLosses;
}

int Player::getTPs() const
{
    return m_TPs;
}

int Player::getVPs() const
{
    return m_VPs;
}

int Player::getVPDiff() const
{
    return m_VPDiff;
}

bool Player::isFirstRoundSet() const
{
    return m_FirstRoundSet;
}

bool Player::hasVotedBestPainted() const
{
    return m_VotedBestPainted;
}

bool Player::hasVotedMostSporting() const
{
    return m_VotedMostSporting;
}

std::string Player::getBestPaintedVote() const
{
    return m_BestPaintedVote;
}

std::string Player::getMostSportingVote() const
{
    return m_MostSportingVote;
}

void Player::setName(const std::string &name)
{
    m_Name = name;
}

void Player::setSeed(int seed)
{
    m_Seed = seed;
}

void Player::addMostSportingVote()
{
    m_MostSportingVotes++;
}

void Player::addBestPaintedArmyVote()
{
    m_BestPaintedArmyVotes++;
}

void Player::removeMostSportingVote()
{
    m_MostSportingVotes--;
}

void Player::removeBestPaintedArmyVote()
{
    m_BestPaintedArmyVotes--;
}

void Player::addWin(int value)
{
    addTPs(value);
    m_NumberWins++;
}

void Player::addDraw(int value)
{
    addTPs(value);
    m_NumberDraws++;
}

void Player::addLoss(int value)
{
    addTPs(value);
    m_NumberLosses++;
}

void Player::addTPs(int points)
{
    m_TPs += points;
}

void Player::addVPs(int points)
{
    m_VPs += points;
}

void Player::addVPDiff(int points)
{
    m_VPDiff += points;
}

void Player::addResult(const std::string &result, int value)
{
    if(result == "Win")
    {
        addWin(value);
    }
    else if(result == "Draw")
    {
        addDraw(value);
    }
    else if(result == "Loss")
    {
        addLoss(value);
    }
}

void Player::setFirstRoundSet(bool set)
{
    m_FirstRoundSet = set;
}

void Player::setVotedBestPainted(bool set)
{
    m_VotedBestPainted = set;
}

void Player::setVotedMostSporting(bool set)
{
    m_VotedMostSporting = set;
}

void Player::setBestPaintedVote(const std::string &name)
{
    m_BestPaintedVote = name;
}

void Player::setMostSportingVote(const std::string &name)
{
    m_MostSportingVote = name;
}

QJsonObject Player::getPlayerData()
{
    QJsonObject object;

    object[m_NameTag.c_str()] = m_Name.c_str();
    object[m_SeedTag.c_str()] = m_Seed;
    object[m_MostSportingTag.c_str()] = m_MostSportingVotes;
    object[m_BestPaintedTag.c_str()] = m_BestPaintedArmyVotes;
    object[m_WinsTag.c_str()] = m_NumberWins;
    object[m_DrawsTag.c_str()] = m_NumberDraws;
    object[m_LossesTag.c_str()] = m_NumberLosses;
    object[m_VPsTag.c_str()] = m_VPs;
    object[m_TPsTag.c_str()] = m_TPs;
    object[m_VPDiffTag.c_str()] = m_VPDiff;
    object[m_FirstRoundSetTag.c_str()] = m_FirstRoundSet;
    object[m_VotedBestPaintedTag.c_str()] = m_VotedBestPainted;
    object[m_VotedMostSportingTag.c_str()] = m_VotedMostSporting;
    object[m_BestPaintedVoteTag.c_str()] = m_BestPaintedVote.c_str();
    object[m_MostSportingVoteTag.c_str()] = m_MostSportingVote.c_str();

    return object;
}

void Player::setPlayerData(QJsonObject object)
{
    m_Name = object[m_NameTag.c_str()].toString().toStdString();
    m_Seed = object[m_SeedTag.c_str()].toInt();
    m_MostSportingVotes = object[m_MostSportingTag.c_str()].toInt();
    m_BestPaintedArmyVotes = object[m_BestPaintedTag.c_str()].toInt();
    m_NumberWins = object[m_WinsTag.c_str()].toInt();
    m_NumberDraws = object[m_DrawsTag.c_str()].toInt();
    m_NumberLosses = object[m_LossesTag.c_str()].toInt();
    m_VPs = object[m_VPsTag.c_str()].toInt();
    m_TPs = object[m_TPsTag.c_str()].toInt();
    m_VPDiff = object[m_VPDiffTag.c_str()].toInt();
    m_FirstRoundSet = object[m_FirstRoundSetTag.c_str()].toBool();
    m_VotedBestPainted = object[m_VotedBestPaintedTag.c_str()].toBool();
    m_VotedMostSporting = object[m_VotedMostSportingTag.c_str()].toBool();
    m_BestPaintedVote = object[m_BestPaintedVoteTag.c_str()].toString().toStdString();
    m_MostSportingVote = object[m_MostSportingVoteTag.c_str()].toString().toStdString();
}
