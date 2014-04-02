#include "base/stats/textstream.hh"

namespace Stats {

TextStream *
initTextStream()
{
    static TextStream text;
    return &text;
}

} // namespace Stats
