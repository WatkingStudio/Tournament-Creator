#include "include/directmatchupswap.h"
#include "ui_directmatchupswap.h"

DirectMatchupSwap::DirectMatchupSwap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DirectMatchupSwap)
{
    ui->setupUi(this);
}

DirectMatchupSwap::~DirectMatchupSwap()
{
    delete ui;
}

void DirectMatchupSwap::SetUp(const std::vector<Player> &playerList, const std::vector<std::pair<Player, Player>> &currentRound)
{
    ResetWidget();

    m_TempPlayerList = playerList;
    //Populate the DropDown with players
    for(auto it : m_TempPlayerList)
    {
        ui->playerAComboBox->addItem(it.getName().c_str());
        ui->playerBComboBox->addItem(it.getName().c_str());
    }
    //Set the temp matchups to current matchups
    m_TempPlayerMatchup = currentRound;
}

void DirectMatchupSwap::on_okPushButton_clicked()
{
    utilDebug("Direct Matchup Ok Clicked");
    Player playerOne;
    Player playerTwo;
    Player playerThree;
    Player playerFour;

    for(auto it : m_TempPlayerList)
    {
        if(it.getName() == ui->newMatchUpOnePlayerAName->text().toStdString())
        {
            playerOne = it;
        }
        else if(it.getName() == ui->newMatchUpOnePlayerBName->text().toStdString())
        {
            playerTwo = it;
        }
        else if(it.getName() == ui->newMatchUpTwoPlayerCName->text().toStdString())
        {
            playerThree = it;
        }
        else if(it.getName() == ui->newMatchUpTwoPlayerDName->text().toStdString())
        {
            playerFour = it;
        }
    }

    this->hide();
    emit SwapComplete(playerOne, playerTwo, playerThree, playerFour);
}

void DirectMatchupSwap::on_playerAComboBox_currentIndexChanged(const QString &arg1)
{
    utilDebug("Player A ComboBox Changed: " + arg1.toStdString());
    for(auto it: m_TempPlayerMatchup)
    {
        if(it.first.getName() == arg1.toStdString())
        {
            ui->newMatchUpOnePlayerAName->setText(arg1);
            ui->newMatchUpTwoPlayerCName->setText(it.second.getName().c_str());
        }
        else if(it.second.getName() == arg1.toStdString())
        {
            ui->newMatchUpOnePlayerAName->setText(arg1);
            ui->newMatchUpTwoPlayerCName->setText(it.first.getName().c_str());
        }
    }
}

void DirectMatchupSwap::on_playerBComboBox_currentIndexChanged(const QString &arg1)
{
    utilDebug("Player B ComboBox Changed: " + arg1.toStdString());
    for(auto it: m_TempPlayerMatchup)
    {
        if(it.first.getName() == arg1.toStdString())
        {
            ui->newMatchUpOnePlayerBName->setText(arg1);
            ui->newMatchUpTwoPlayerDName->setText(it.second.getName().c_str());
        }
        else if(it.second.getName() == arg1.toStdString())
        {
            ui->newMatchUpOnePlayerBName->setText(arg1);
            ui->newMatchUpTwoPlayerDName->setText(it.first.getName().c_str());
        }
    }
}

void DirectMatchupSwap::ResetWidget()
{
    ui->playerAComboBox->setCurrentIndex(0);
    ui->playerBComboBox->setCurrentIndex(0);

    ui->newMatchUpOnePlayerAName->setText("");
    ui->newMatchUpOnePlayerBName->setText("");
    ui->newMatchUpTwoPlayerCName->setText("");
    ui->newMatchUpTwoPlayerDName->setText("");
}
