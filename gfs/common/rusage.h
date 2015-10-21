#ifndef COMMON_RUSAGE_H_
#define COMMON_RUSAGE_H_

#include <ostream>

namespace GFS
{
using std::ostream;

ostream& showrusage(ostream& inStream, const char* inSeparatorPtr,
    const char* inDelimiterPtr, bool inSelfFlag);
}

#endif /* RUSAGE_H */
