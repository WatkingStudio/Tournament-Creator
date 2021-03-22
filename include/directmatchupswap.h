#ifndef DIRECTMATCHUPSWAP_H
#define DIRECTMATCHUPSWAP_H

#include <QWidget>
#include "player.h"

namespace Ui {
class DirectMatchupSwap;
}

// This class handles the functionality to setup a direct swap to have two player play each other
class DirectMatchupSwap : public QWidget
{
    Q_OBJECT

public:
    explicit DirectMatchupSwap(QWidget *parent = 0);
    ~DirectMatchupSwap();

    // Set up the Direct Matchup Widget
    void SetUp(const std::vector<Player> &playerList, const std::vector<std::pair<Player, Player>> &currentRound);

signals:
    void SwapComplete(const Player &playerOne, const Player &playerTwo, const Player &playerThree, const Player &playerFour);

private slots:
    // The result of clicking the Ok button
    void on_OkPushButton_clicked();

    // The result of changing the PlayerA combo box
    void on_PlayerAComboBox_currentIndexChanged(const QString &arg1);

    //The result of changing the PlayerB combo box
    void on_PlayerBComboBox_currentIndexChanged(const QString &arg1);

    // The result of clicking the Cancel button
    void on_CancelPushButton_clicked();

private:
    std::unique_ptr<Ui::DirectMatchupSwap> m_Ui;

    std::unique_ptr<std::vector<std::pair<Player, Player>>> m_TempPlayerMatchup = std::make_unique<std::vector<std::pair<Player, Player>>>();
    std::unique_ptr<std::vector<Player>> m_TempPlayerList = std::make_unique<std::vector<Player>>();

    // Checks that the input players are valid
    bool CheckInputsAreValid() const;

    // Resets the Widget
    void ResetWidget();
};

#endif // DIRECTMATCHUPSWAP_H
