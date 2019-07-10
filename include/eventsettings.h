#ifndef EVENTSETTINGS_H
#define EVENTSETTINGS_H

#include <QWidget>

namespace Ui {
class EventSettings;
}

class EventSettings : public QWidget
{
    Q_OBJECT

public:
    explicit EventSettings(QWidget *parent = 0);
    ~EventSettings();

private slots:
    void on_donePushButton_clicked();

private:
    Ui::EventSettings *ui;
};

#endif // EVENTSETTINGS_H
