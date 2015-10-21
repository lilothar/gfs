#include "nofilelimit.h"
#include <sys/resource.h>
#include <errno.h>
#include <walle/sys/wallesys.h>

namespace KFS
{

int
SetMaxNoFileLimit()
{
    struct rlimit theRlim       = {0};
    int           theMaxOpenFds = 0;

    // bump the soft limit to the hard limit
    if (getrlimit(RLIMIT_NOFILE, &theRlim) == 0) {
        const rlim_t kMaxFdLimit = 1 << 20;
        int          theErr      = 0;
        if (theRlim.rlim_cur >= kMaxFdLimit) {
            theMaxOpenFds = (int)kMaxFdLimit;
        } else {
            theMaxOpenFds = (int)theRlim.rlim_cur;
            if (theRlim.rlim_max > kMaxFdLimit) {
                theRlim.rlim_cur = kMaxFdLimit;
            } else {
                theRlim.rlim_cur = theRlim.rlim_max;
            }
            while (theMaxOpenFds < (int)theRlim.rlim_cur &&
                   setrlimit(RLIMIT_NOFILE, &theRlim) != 0) {
                theErr = errno;
                theRlim.rlim_cur /= 2;
            }
            if ((int)theRlim.rlim_cur > theMaxOpenFds) {
                theMaxOpenFds = (int)theRlim.rlim_cur;
            }
            if (theErr) {
                LOG_ERROR<<
                    "setrilimit(RLIMIT_NOFILE, " <<
                    theRlim.rlim_cur * 2 <<
                    "): " <<theErr);
            }
        }
    } else {
        const int theErr = errno;
        LOG_ERROR <<
            "getrlimit(RLIMIT_NOFILE): " <<theErr;
        }
    LOG_INFO <<
        "max # of open files: " << theMaxOpenFds;
    return theMaxOpenFds;
}
    
}
