#ifndef QCUTILS_H
#define QCUTILS_H

#include <stdint.h>
#include <string>

#define QCRTASSERT(a) \
    if (!(a)) QCUtils::AssertionFailure(#a, __FILE__, __LINE__)

struct QCUtils
{
    static void FatalError(
        const char* inMsgPtr,
        int         inSysError);
    static std::string SysError(
        int         inSysError,
        const char* inMsgPtr = 0);
    static void AssertionFailure(
        const char* inMsgPtr,
        const char* inFileNamePtr,
        int         inLineNum);
    static int64_t ReserveFileSpace(
        int     inFd,
        int64_t inSize);
    static int AllocateFileSpace(
        const char* inFileNamePtr,
        int64_t     inSize,
        int64_t     inMinSize            = -1,
        int64_t*    inInitialFileSizePtr = 0);
    static int AllocateFileSpace(
        int      inFd,
        int64_t  inSize,
        int64_t  inMinSize            = -1,
        int64_t* inInitialFileSizePtr = 0);
};

#endif /* QCUTILS_H */
