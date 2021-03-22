#include "include/directmatchupswap.h"
#include "ui_directmatchupswap.h"
#include <QMessageBox>

DirectMatchupSwap::DirectMatchupSwap(QWidget *parent) :
    QWidget(parent),
    m_Ui(new Ui::DirectMatchupSwap)
{
    m_Ui->setupUi(this);
}

DirectMatchupSwap::~DirectMatchupSwap()
{

}

bool DirectMatchupSwap::CheckInputsAreValid() const
{
    if(m_Ui->PlayerAComboBox->currentText() == m_Ui->PlayerBComboBox->currentText()
            || m_Ui->PlayerAComboBox->currentText() == ""
            || m_Ui->PlayerBComboBox->currentText() == "")
    {
        return false;
    }
    else if(m_Ui->NewMatchUpOnePlayerAName->text() == m_Ui->NewMatchUpTwoPlayerCName->text()
            || m_Ui->NewMatchUpOnePlayerAName->text() == m_Ui->NewMatchUpTwoPlayerDName->text())
    {
        return false;
    }
    return true;
}

void DirectMatchupSwap::on_CancelPushButton_clicked()
{
    this->hide();
}

void DirectMatchupSwap::on_OkPushButton_clicked()
{
    UtilDebug("Direct Matchup Ok Clicked");

    if(CheckInputsAreValid())
    {
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
    else
    {
        QMessageBox::warning(this, "Invalid Players", "The players you wish to have a matchup are not valid. Please ensure that you have entered the data correctly before continuing.");
    }
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
    m_Ui->PlayerAComboBox->clear();
    m_Ui->PlayerBComboBox->clear();
    m_Ui->PlayerAComboBox->addItem("");
    m_Ui->PlayerBComboBox->addItem("");

    m_Ui->NewMatchUpOnePlayerAName->setText("");
    m_Ui->NewMatchUpOnePlayerBName->setText("");
    m_Ui->NewMatchUpTwoPlayerCName->setText("");
    m_Ui->NewMatchUpTwoPlayerDName->setText("");
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
