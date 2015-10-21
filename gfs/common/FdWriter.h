#ifndef COMMON_FD_WRITER_H_
#define COMMON_FD_WRITER_H_

#include <unistd.h>
#include <errno.h>

namespace GFS
{

class FdWriter
{
public:
    FdWriter(
        int inFd)
        : _fd(inFd),
          _error(0)
        {}
    ~FdWriter()
        {}
    void flush()
        {}
    bool write(
        const void* inBufPtr,
        size_t      inLength)
    {
        const char*       thePtr    = static_cast<const char*>(inBufPtr);
        const char* const theEndPtr = thePtr + inLength;
        while (thePtr < theEndPtr) {
            const ssize_t theNWr = ::write(_fd, thePtr, theEndPtr - thePtr);
            if (theNWr < 0 && errno != EINTR) {
                _error = errno;
                return false;
            }
            thePtr += theNWr;
        }
        return true;
    }
    void ClearError()
        { _error = 0; }
    int GetError() const
        { return _error; }
private:
    const int _fd;
    int       _error;
};

}

#endif 
