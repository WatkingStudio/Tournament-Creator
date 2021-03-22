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
        m_Ui->PlayerAComboBox->addItem(it.GetName().c_str());
        m_Ui->PlayerBComboBox->addItem(it.GetName().c_str());
    }
    //Set the temp matchups to current matchups
    *m_TempPlayerMatchup = currentRound;
}

void DirectMatchupSwap::on_OkPushButton_clicked()
{
    UtilDebug("Direct Matchup Ok Clicked");

    std::vector<Player>::iterator playerOne;
    std::vector<Player>::iterator playerTwo;
    std::vector<Player>::iterator playerThree;
    std::vector<Player>::iterator playerFour;

    for(auto it = m_TempPlayerList->begin(); it != m_TempPlayerList->end(); it++)
    {
        if(it->GetName() == m_Ui->NewMatchUpOnePlayerAName->text().toStdString())
        {
            playerOne = it;
        }
        else if(it->GetName() == m_Ui->NewMatchUpOnePlayerBName->text().toStdString())
        {
            playerTwo = it;
        }
        else if(it->GetName() == m_Ui->NewMatchUpTwoPlayerCName->text().toStdString())
        {
            playerThree = it;
        }
        else if(it->GetName() == m_Ui->NewMatchUpTwoPlayerDName->text().toStdString())
        {
            playerFour = it;
        }
    }

    this->hide();
    emit SwapComplete(*playerOne, *playerTwo, *playerThree, *playerFour);
}

void DirectMatchupSwap::on_PlayerAComboBox_currentIndexChanged(const QString &arg1)
{
    UtilDebug("Player A ComboBox Changed: " + arg1.toStdString());
    for(const auto &it: *m_TempPlayerMatchup)
    {
        if(it.first.GetName() == arg1.toStdString())
        {
            m_Ui->NewMatchUpOnePlayerAName->setText(arg1);
            m_Ui->NewMatchUpTwoPlayerCName->setText(it.second.GetName().c_str());
        }
        else if(it.second.GetName() == arg1.toStdString())
        {
            m_Ui->NewMatchUpOnePlayerAName->setText(arg1);
            m_Ui->NewMatchUpTwoPlayerCName->setText(it.first.GetName().c_str());
        }
    }
}

void DirectMatchupSwap::on_PlayerBComboBox_currentIndexChanged(const QString &arg1)
{
    UtilDebug("Player B ComboBox Changed: " + arg1.toStdString());
    for(const auto &it: *m_TempPlayerMatchup)
    {
        if(it.first.GetName() == arg1.toStdString())
        {
            m_Ui->NewMatchUpOnePlayerBName->setText(arg1);
            m_Ui->NewMatchUpTwoPlayerDName->setText(it.second.GetName().c_str());
        }
        else if(it.second.GetName() == arg1.toStdString())
        {
            m_Ui->NewMatchUpOnePlayerBName->setText(arg1);
            m_Ui->NewMatchUpTwoPlayerDName->setText(it.first.GetName().c_str());
        }
    }
}

void DirectMatchupSwap::ResetWidget()
{
    m_Ui->PlayerAComboBox->setCurrentIndex(0);
    m_Ui->PlayerBComboBox->setCurrentIndex(0);

    m_Ui->NewMatchUpOnePlayerAName->setText("");
    m_Ui->NewMatchUpOnePlayerBName->setText("");
    m_Ui->NewMatchUpTwoPlayerCName->setText("");
    m_Ui->NewMatchUpTwoPlayerDName->setText("");
}

void DirectMatchupSwap::on_CancelPushButton_clicked()
{
    this->hide();
}
