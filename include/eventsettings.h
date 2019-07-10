#ifndef EVENTSETTINGS_H
#define EVENTSETTINGS_H

#include <QWidget>
#include "util.h"
#include "tiebreakerdata.h"

namespace Ui {
class EventSettings;
}

class EventSettings : public QWidget
{
    Q_OBJECT

public:
    explicit EventSettings(QWidget *parent = 0);
    ~EventSettings();

    void SetupWidget();
    void PopulateComboBox();

signals:
    void SettingsComplete(int winTPs, int drawTPs, int lossTPs, int mostSportingTPs, int bestPaintedTPs, bool usingSeeded, int numberOfRounds, std::string firstTiebreaker, std::string secondTiebreaker, std::string thirdTiebreaker, std::string fourthTiebreaker);

private slots:
    void on_donePushButton_clicked();

private:
    Ui::EventSettings *ui;
};

#endif // EVENTSETTINGS_H
