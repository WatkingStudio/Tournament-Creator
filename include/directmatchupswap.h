#ifndef DIRECTMATCHUPSWAP_H
#define DIRECTMATCHUPSWAP_H

#include <QWidget>
#include "player.h"

namespace Ui {
class DirectMatchupSwap;
}

class DirectMatchupSwap : public QWidget
{
    Q_OBJECT

public:
    explicit DirectMatchupSwap(QWidget *parent = 0);
    ~DirectMatchupSwap();

    void SetUp(const std::vector<Player> &playerList, const std::vector<std::pair<Player, Player>> &currentRound);

signals:
    void SwapComplete(Player playerOne, Player playerTwo, Player playerThree, Player playerFour);

private slots:
    void on_okPushButton_clicked();

    void on_playerAComboBox_currentIndexChanged(const QString &arg1);

    void on_playerBComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::DirectMatchupSwap *ui;

    std::vector<std::pair<Player, Player>> m_TempPlayerMatchup;
    std::vector<Player> m_TempPlayerList;
};

#endif // DIRECTMATCHUPSWAP_H
