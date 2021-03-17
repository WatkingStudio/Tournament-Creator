#include "include/mainwindow.h"
#include <QApplication>
#include "include/tiebreakerdata.h"

namespace Tiebreak {
    std::unique_ptr<std::string> VPTotalString;
    std::unique_ptr<std::string> VPDiffString;
    std::unique_ptr<std::string> MostSportingString;
    std::unique_ptr<std::string> BestPaintedString;
    std::unique_ptr<std::string> NoneString;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Tiebreak::NoneString = std::make_unique<std::string>("None");
    Tiebreak::VPTotalString = std::make_unique<std::string>("VP Total");
    Tiebreak::VPDiffString = std::make_unique<std::string>("VP Diff");
    Tiebreak::MostSportingString = std::make_unique<std::string>("Most Sporting");
    Tiebreak::BestPaintedString = std::make_unique<std::string>("Best Painted");

    MainWindow w;
    w.show();

    return a.exec();
}
