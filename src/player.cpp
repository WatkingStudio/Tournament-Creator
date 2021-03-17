#include "include\player.h"
#include <string>

Player::Player(const std::string &name, const uint seed)
{
    *m_Name = name;
    *m_Seed = seed;
}

Player::Player(const Player &player)
{
    *m_Name = player.GetName();
    *m_NumberDraws = player.GetDraws();
    *m_NumberLosses = player.GetLosses();
    *m_NumberWins = player.GetWins();
    *m_Seed = player.GetSeed();

    *m_TPs = player.GetTPs();
    *m_VPDiff = player.GetVPDiff();
    *m_VPs = player.GetVPs();

    *m_BestPaintedVote = player.GetBestPaintedVote();
    *m_BestPaintedArmyVotes = player.GetBestPaintedArmyVotes();
    *m_VotedMostSporting = player.HasVotedMostSporting();
    *m_MostSportingVotes = player.GetMostSportingVotes();
    *m_VotedBestPainted = player.HasVotedBestPainted();
    *m_MostSportingVote = player.GetMostSportingVote();

    *m_FirstRoundSet = player.IsFirstRoundSet();
}

void Player::AddBestPaintedArmyVote(const int value)
{
    (*m_BestPaintedArmyVotes)++;
    *m_TPs += value;
}

void Player::AddDraw(const int value)
{
    AddTPs(value);
    (*m_NumberDraws)++;
}

void Player::AddLoss(const int value)
{
    AddTPs(value);
    (*m_NumberLosses)++;
}

void Player::AddMostSportingVote(const int value)
{
    (*m_MostSportingVotes)++;
    *m_TPs += value;
}

void Player::AddResult(const std::string &result, int value)
{
    UtilDebug("Add Result: " + *m_Name + " " + result);
    if(result == "Win")
    {
        AddWin(value);
    }
    else if(result == "Draw")
    {
        AddDraw(value);
    }
    else if(result == "Loss")
    {
        AddLoss(value);
    }
}

void Player::AddTPs(const int points)
{
    *m_TPs += points;
}

void Player::AddVPDiff(const int points)
{
    *m_VPDiff += points;
}

void Player::AddVPs(const int points)
{
    *m_VPs += points;
}

void Player::AddWin(const int value)
{
    AddTPs(value);
    (*m_NumberWins)++;
}

std::string Player::GetBestPaintedVote() const
{
    return *m_BestPaintedVote;
}

int Player::GetBestPaintedArmyVotes() const
{
    return *m_BestPaintedArmyVotes;
}

int Player::GetDraws() const
{
    return *m_NumberDraws;
}

int Player::GetLosses() const
{
    return *m_NumberLosses;
}

std::string Player::GetMostSportingVote() const
{
    return *m_MostSportingVote;
}

int Player::GetMostSportingVotes() const
{
    return *m_MostSportingVotes;
}

std::string Player::GetName() const
{
    return *m_Name;
}

QJsonObject Player::GetPlayerData() const
{
    QJsonObject object;

    object[m_NameTag->c_str()] = m_Name->c_str();
    object[m_DrawsTag->c_str()] = *m_NumberDraws;
    object[m_WinsTag->c_str()] = *m_NumberWins;
    object[m_LossesTag->c_str()] = *m_NumberLosses;
    object[m_SeedTag->c_str()] = *m_Seed;

    object[m_TPsTag->c_str()] = *m_TPs;
    object[m_VPDiffTag->c_str()] = *m_VPDiff;
    object[m_VPsTag->c_str()] = *m_VPs;

    object[m_BestPaintedTag->c_str()] = *m_BestPaintedArmyVotes;
    object[m_BestPaintedVoteTag->c_str()] = m_BestPaintedVote->c_str();
    object[m_MostSportingTag->c_str()] = *m_MostSportingVotes;
    object[m_MostSportingVoteTag->c_str()] = m_MostSportingVote->c_str();
    object[m_VotedBestPaintedTag->c_str()] = *m_VotedBestPainted;
    object[m_VotedMostSportingTag->c_str()] = *m_VotedMostSporting;

    object[m_FirstRoundSetTag->c_str()] = *m_FirstRoundSet;

    return object;
}

int Player::GetSeed() const
{
    return *m_Seed;
}

int Player::GetTPs() const
{
    return *m_TPs;
}

int Player::GetVPDiff() const
{
    return *m_VPDiff;
}

int Player::GetVPs() const
{
    return *m_VPs;
}

int Player::GetWins() const
{
    return *m_NumberWins;
}

void Player::operator=(const Player &player)
{
    *m_Name = player.GetName();
    *m_NumberDraws = player.GetDraws();
    *m_NumberLosses = player.GetLosses();
    *m_NumberWins = player.GetWins();
    *m_Seed = player.GetSeed();

    *m_TPs = player.GetTPs();
    *m_VPDiff = player.GetVPDiff();
    *m_VPs = player.GetVPs();

    *m_BestPaintedVote = player.GetBestPaintedVote();
    *m_BestPaintedArmyVotes = player.GetBestPaintedArmyVotes();
    *m_MostSportingVote = player.GetMostSportingVote();
    *m_MostSportingVotes = player.GetMostSportingVotes();
    *m_VotedBestPainted = player.HasVotedBestPainted();
    *m_VotedMostSporting = player.HasVotedMostSporting();

    *m_FirstRoundSet = player.IsFirstRoundSet();
}

bool Player::HasVotedBestPainted() const
{
    return *m_VotedBestPainted;
}

bool Player::HasVotedMostSporting() const
{
    return *m_VotedMostSporting;
}

bool Player::IsFirstRoundSet() const
{
    return *m_FirstRoundSet;
}

void Player::RemoveBestPaintedArmyVote(const int value)
{
    (*m_BestPaintedArmyVotes)--;
    *m_TPs -= value;
}

void Player::RemoveMostSportingVote(const int value)
{
    (*m_MostSportingVotes)--;
    *m_TPs -= value;
}

void Player::SetBestPaintedVote(const std::string &name)
{
    *m_BestPaintedVote = name;
}

void Player::SetFirstRoundSet(const bool set)
{
    *m_FirstRoundSet = set;
}

void Player::SetMostSportingVote(const std::string &name)
{
    *m_MostSportingVote = name;
}

void Player::SetName(const std::string &name)
{
    *m_Name = name;
}

void Player::SetPlayerData(const QJsonObject &object)
{
    *m_Name = object[m_NameTag->c_str()].toString().toStdString();
    *m_NumberDraws = object[m_DrawsTag->c_str()].toInt();
    *m_NumberLosses = object[m_LossesTag->c_str()].toInt();
    *m_NumberWins = object[m_WinsTag->c_str()].toInt();
    *m_Seed = object[m_SeedTag->c_str()].toInt();

    *m_TPs = object[m_TPsTag->c_str()].toInt();
    *m_VPDiff = object[m_VPDiffTag->c_str()].toInt();
    *m_VPs = object[m_VPsTag->c_str()].toInt();

    *m_BestPaintedVote = object[m_BestPaintedVoteTag->c_str()].toString().toStdString();
    *m_BestPaintedArmyVotes = object[m_BestPaintedTag->c_str()].toInt();
    *m_MostSportingVote = object[m_MostSportingVoteTag->c_str()].toString().toStdString();
    *m_MostSportingVotes = object[m_MostSportingTag->c_str()].toInt();
    *m_VotedBestPainted = object[m_VotedBestPaintedTag->c_str()].toBool();
    *m_VotedMostSporting = object[m_VotedMostSportingTag->c_str()].toBool();

    *m_FirstRoundSet = object[m_FirstRoundSetTag->c_str()].toBool();
}

void Player::SetSeed(const int seed)
{
    *m_Seed = seed;
}

void Player::SetVotedBestPainted(const bool set)
{
    *m_VotedBestPainted = set;
}

void Player::SetVotedMostSporting(const bool set)
{
    *m_VotedMostSporting = set;
}
