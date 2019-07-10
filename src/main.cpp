#include "include/mainwindow.h"
#include <QApplication>
#include "include/tiebreakerdata.h"

namespace Tiebreak {
    std::shared_ptr<std::string> VPTotalString;
    std::shared_ptr<std::string> VPDiffString;
    std::shared_ptr<std::string> MostSportingString;
    std::shared_ptr<std::string> BestPaintedString;
    std::shared_ptr<std::string> NoneString;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Tiebreak::NoneString = std::make_shared<std::string>("None");
    Tiebreak::VPTotalString = std::make_shared<std::string>("VP Total");
    Tiebreak::VPDiffString = std::make_shared<std::string>("VP Diff");
    Tiebreak::MostSportingString = std::make_shared<std::string>("Most Sporting");
    Tiebreak::BestPaintedString = std::make_shared<std::string>("Best Painted");

    MainWindow w;
    w.show();

    return a.exec();
}
