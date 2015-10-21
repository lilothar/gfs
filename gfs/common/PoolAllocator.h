#ifndef COMMON_POOL_ALLOCATOR_H
#define COMMON_POOL_ALLOCATOR_H

#include <stddef.h>
#include <assert.h>
#include <string.h>

#include <algorithm>

namespace KFS
{

using std::max;
using std::min;

template<
    size_t TItemSize,
    size_t TMinStorageAlloc,
    size_t TMaxStorageAlloc,
    bool   TForceCleanupFlag
>
class PoolAllocator
{
public:
    PoolAllocator()
        : _freeStoragePtr(0),
          _freeStorageEndPtr(0),
          _storageListPtr(0),
          _freeListPtr(0),
          _allocSize(max(TMinStorageAlloc, getElemSize())),
          _storageSize(0),
          _inUseCount(0)
        {}
    ~PoolAllocator()
    {
        if (! TForceCleanupFlag && _inUseCount > 0) {
            return; // Memory leak
        }
        while (_storageListPtr) {
            char* const theCurPtr = _storageListPtr;
            char** thePtr = reinterpret_cast<char**>(theCurPtr);
            _storageListPtr = *thePtr++;
            assert(*thePtr == theCurPtr);
            delete [] *thePtr;
        }
    }
    char* allocate()
    {
        if (_freeListPtr) {
            _inUseCount++;
            return getNextFree();
        }
        char* theEndPtr = _freeStoragePtr + getElemSize();
        if (theEndPtr > _freeStorageEndPtr) {
            // Maintain 2 * sizeof(size_t) alignment.
            const size_t theHdrSize = 2 * sizeof(_storageListPtr);
            const size_t theSize    = _allocSize + 2 * sizeof(_storageListPtr);
            _freeStoragePtr    = new char[theSize];
            _freeStorageEndPtr = _freeStoragePtr + theSize;
            char** thePtr = reinterpret_cast<char**>(_freeStoragePtr);
            *thePtr++ = _storageListPtr;
            *thePtr   = _freeStoragePtr; // store ptr to catch buffer overrun.
            _storageListPtr = _freeStoragePtr;
            _freeStoragePtr += theHdrSize;
            _allocSize = min(TMaxStorageAlloc, _allocSize << 1);
            _storageSize += theSize;
            theEndPtr = _freeStoragePtr + getElemSize();
        }
        char* const theRetPtr = _freeStoragePtr;
        _freeStoragePtr = theEndPtr;
        _inUseCount++;
        return theRetPtr;
    }
    void deallocate(
        void* inPtr)
    {
        if (! inPtr) {
            return;
        }
        assert(_inUseCount > 0);
        _inUseCount--;
        put(inPtr);
    }
    size_t getInUseCount() const
        { return _inUseCount; }
    size_t getStorageSize() const
        { return _storageSize; }
    static size_t getItemSize()
        { return TItemSize; }
    static size_t getElemSize()
        { return max(TItemSize, sizeof(char*)); }
private:
    char*  _freeStoragePtr;
    char*  _freeStorageEndPtr;
    char*  _storageListPtr;
    char*  _freeListPtr;
    size_t _allocSize;
    size_t _storageSize;
    size_t _inUseCount;

    char* getNextFree()
    {
        char* const theRetPtr = _freeListPtr;
        if (getElemSize() % sizeof(_freeListPtr) == 0) {
            _freeListPtr = *reinterpret_cast<char**>(theRetPtr);
        } else {
            memcpy(&_freeListPtr, theRetPtr, sizeof(_freeListPtr));
        }
        return theRetPtr;
    }
    void put(void* inPtr)
    {
        if (getElemSize() % sizeof(_freeListPtr) == 0) {
            *reinterpret_cast<char**>(inPtr) = _freeListPtr;
        } else {
            memcpy(inPtr, &_freeListPtr, sizeof(_freeListPtr));
        }
        _freeListPtr = reinterpret_cast<char*>(inPtr);
    }

    PoolAllocator( PoolAllocator& inAlloc);
    PoolAllocator& operator=( PoolAllocator& inAlloc);
};

}

#endif /* POOL_ALLOCATOR_H */
