#include "include/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_createNewEventButton_clicked()
{
    loadTournamentCreatorPage();
}

void MainWindow::loadStartPage()
{
    utilDebug("Load Start Page");
    ui->stackedWidget->setCurrentIndex(Pages::START_SCREEN);
    ui->pageTitleWidget->setText(QString("Start Page"));
}

void MainWindow::on_loadEventButton_clicked()
{
    loadEventData();
    utilLog("Debug Point");
    loadMatchupsPageFromLoadedEvent();
}
