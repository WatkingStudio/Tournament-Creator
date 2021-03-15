#include "include/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_createNewEventButton_clicked()
{
    LoadTournamentCreatorPage();
}

void MainWindow::loadStartPage()
{
    UtilDebug("Load Start Page");
    ui->stackedWidget->setCurrentIndex(Pages::START_SCREEN);
    ui->pageTitleWidget->setText(QString("Start Page"));
}

void MainWindow::on_loadEventButton_clicked()
{
    UtilDebug("Load Event Clicked");
    if(loadEventData())
        loadMatchupsPageFromLoadedEvent();
}
