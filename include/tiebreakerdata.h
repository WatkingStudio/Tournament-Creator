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

extern std::unique_ptr<std::string> VPTotalString;
extern std::unique_ptr<std::string> VPDiffString;
extern std::unique_ptr<std::string> MostSportingString;
extern std::unique_ptr<std::string> BestPaintedString;
extern std::unique_ptr<std::string> NoneString;

// Converts a string into a Tiebreaker enum
Tiebreaker StringToTiebreaker(const std::string &string);

// Converts a Tiebreaker enum into a string
std::string TiebreakerToString(Tiebreaker tiebreaker);

}

#endif // TIEBREAKERDATA_H
