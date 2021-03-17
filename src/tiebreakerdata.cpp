#include "include/tiebreakerdata.h"

namespace Tiebreak {

Tiebreaker StringToTiebreaker(const std::string &string)
{
    if(string == *VPTotalString)
        return Tiebreaker::VP_TOTAL;
    else if(string == *VPDiffString)
        return Tiebreaker::VP_DIFF;
    else if(string == *MostSportingString)
        return Tiebreaker::MOST_SPORTING;
    else if(string == *BestPaintedString)
        return Tiebreaker::BEST_PAINTED;
    else
        return Tiebreaker::NONE;
}

std::string TiebreakerToString(Tiebreaker tiebreaker)
{
    switch(tiebreaker)
    {
    case Tiebreaker::VP_TOTAL:
        return *VPTotalString;
    case Tiebreaker::VP_DIFF:
        return *VPDiffString;
    case Tiebreaker::MOST_SPORTING:
        return *MostSportingString;
    case Tiebreaker::BEST_PAINTED:
        return *BestPaintedString;
    default:
        return *NoneString;
    }
}

}
