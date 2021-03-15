#include "include/directmatchupswap.h"
#include "ui_directmatchupswap.h"

DirectMatchupSwap::DirectMatchupSwap(QWidget *parent) :
    QWidget(parent),
    m_Ui(new Ui::DirectMatchupSwap)
{
    m_Ui->setupUi(this);
}

DirectMatchupSwap::~DirectMatchupSwap()
{

}

void DirectMatchupSwap::SetUp(const std::vector<Player> &playerList, const std::vector<std::pair<Player, Player>> &currentRound)
{
    ResetWidget();

    *m_TempPlayerList = playerList;
    //Populate the DropDown with players
    for(const auto &it : *m_TempPlayerList)
    {
        m_Ui->PlayerAComboBox->addItem(it.getName().c_str());
        m_Ui->PlayerBComboBox->addItem(it.getName().c_str());
    }
    //Set the temp matchups to current matchups
    *m_TempPlayerMatchup = currentRound;
}

void DirectMatchupSwap::on_OkPushButton_clicked()
{
    UtilDebug("Direct Matchup Ok Clicked");
    Player playerOne;
    Player playerTwo;
    Player playerThree;
    Player playerFour;

    for(const auto &it : *m_TempPlayerList)
    {
        if(it.getName() == m_Ui->newMatchUpOnePlayerAName->text().toStdString())
        {
            playerOne = it;
        }
        else if(it.getName() == m_Ui->newMatchUpOnePlayerBName->text().toStdString())
        {
            playerTwo = it;
        }
        else if(it.getName() == m_Ui->newMatchUpTwoPlayerCName->text().toStdString())
        {
            playerThree = it;
        }
        else if(it.getName() == m_Ui->newMatchUpTwoPlayerDName->text().toStdString())
        {
            playerFour = it;
        }
    }

    this->hide();
    emit SwapComplete(playerOne, playerTwo, playerThree, playerFour);
}

void DirectMatchupSwap::on_PlayerAComboBox_currentIndexChanged(const QString &arg1)
{
    UtilDebug("Player A ComboBox Changed: " + arg1.toStdString());
    for(const auto &it: *m_TempPlayerMatchup)
    {
        if(it.first.getName() == arg1.toStdString())
        {
            m_Ui->newMatchUpOnePlayerAName->setText(arg1);
            m_Ui->newMatchUpTwoPlayerCName->setText(it.second.getName().c_str());
        }
        else if(it.second.getName() == arg1.toStdString())
        {
            m_Ui->newMatchUpOnePlayerAName->setText(arg1);
            m_Ui->newMatchUpTwoPlayerCName->setText(it.first.getName().c_str());
        }
    }
}

void DirectMatchupSwap::on_PlayerBComboBox_currentIndexChanged(const QString &arg1)
{
    UtilDebug("Player B ComboBox Changed: " + arg1.toStdString());
    for(const auto &it: *m_TempPlayerMatchup)
    {
        if(it.first.getName() == arg1.toStdString())
        {
            m_Ui->newMatchUpOnePlayerBName->setText(arg1);
            m_Ui->newMatchUpTwoPlayerDName->setText(it.second.getName().c_str());
        }
        else if(it.second.getName() == arg1.toStdString())
        {
            m_Ui->newMatchUpOnePlayerBName->setText(arg1);
            m_Ui->newMatchUpTwoPlayerDName->setText(it.first.getName().c_str());
        }
    }
}

void DirectMatchupSwap::ResetWidget()
{
    m_Ui->PlayerAComboBox->setCurrentIndex(0);
    m_Ui->PlayerBComboBox->setCurrentIndex(0);

    m_Ui->newMatchUpOnePlayerAName->setText("");
    m_Ui->newMatchUpOnePlayerBName->setText("");
    m_Ui->newMatchUpTwoPlayerCName->setText("");
    m_Ui->newMatchUpTwoPlayerDName->setText("");
}

void DirectMatchupSwap::on_CancelPushButton_clicked()
{
    this->hide();
}
