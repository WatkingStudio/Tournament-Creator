#ifndef TIEBREAKERDATA_H
#define TIEBREAKERDATA_H

#include <string>
#include <memory>

namespace Tiebreak {

enum Tiebreaker{
    VP_TOTAL,
    VP_DIFF,
    MOST_SPORTING,
    BEST_PAINTED,
    NONE
};

extern std::shared_ptr<std::string> VPTotalString;
extern std::shared_ptr<std::string> VPDiffString;
extern std::shared_ptr<std::string> MostSportingString;
extern std::shared_ptr<std::string> BestPaintedString;
extern std::shared_ptr<std::string> NoneString;

std::string TiebreakerToString(Tiebreaker tiebreaker);
Tiebreaker StringToTiebreaker(const std::string &string);

}

#endif // TIEBREAKERDATA_H
