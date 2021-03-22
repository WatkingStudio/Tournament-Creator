#include "include/mainwindow.h"
#include "ui_mainwindow.h"

// This .cpp file is used to handle the start page

void MainWindow::LoadStartPage()
{
    UtilDebug("Load Start Page");
    m_Ui->StackedWidget->setCurrentIndex(Pages::START_SCREEN);
    m_Ui->PageTitleWidget->setText(QString("Start Page"));
}

void MainWindow::on_CreateNewEventButton_clicked()
{
    LoadTournamentCreatorPage();
}

void MainWindow::on_LoadEventButton_clicked()
{
    UtilDebug("Load Event Clicked");
    if(LoadEventData())
    {
        LoadMatchupsPageFromLoadedEvent();
    }
}
