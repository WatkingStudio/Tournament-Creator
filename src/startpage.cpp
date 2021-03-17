#include "include/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::LoadStartPage()
{
    UtilDebug("Load Start Page");
    ui->stackedWidget->setCurrentIndex(Pages::START_SCREEN);
    ui->pageTitleWidget->setText(QString("Start Page"));
}

void MainWindow::on_CreateNewEventButton_clicked()
{
    LoadTournamentCreatorPage();
}

void MainWindow::on_LoadEventButton_clicked()
{
    UtilDebug("Load Event Clicked");
    if(loadEventData())
    {
        LoadMatchupsPageFromLoadedEvent();
    }
}
