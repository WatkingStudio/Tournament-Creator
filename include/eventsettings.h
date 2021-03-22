#ifndef EVENTSETTINGS_H
#define EVENTSETTINGS_H

#include <QWidget>
#include "util.h"
#include "tiebreakerdata.h"

namespace Ui {
class EventSettings;
}

struct EventSettingsData
{
    int winTPs = 0;
    int drawTPs = 0;
    int lossTPs = 0;
    int mostSportingTPs = 0;
    int bestPaintedTPs = 0;
    bool usingSeeded = false;
    int numberOfRounds = 0;
    std::string firstTiebreaker;
    std::string secondTiebreaker;
    std::string thirdTiebreaker;
    std::string fourthTiebreaker;
};

//This class handles the Event Settings widget
class EventSettings : public QWidget
{
    Q_OBJECT

public:
    explicit EventSettings(QWidget *parent = 0);
    ~EventSettings();

    // Setup the Event Settings Widget
    void SetupWidget();

    // Populate the combo boxes
    void PopulateComboBox();

signals:
    void SettingsComplete(const EventSettingsData &settings);

private slots:
    // Do this when the Done button is clicked
    void on_DonePushButton_clicked();

private:
    Ui::EventSettings *m_Ui;

    // Check to see if the set tiebreakers are unique
    // @return True if the tiebreakers are unique, False if they are not
    bool CheckUniqueTiebreakers() const;
};

#endif // EVENTSETTINGS_H
