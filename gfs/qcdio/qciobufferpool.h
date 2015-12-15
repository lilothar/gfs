#ifndef IOBUFFERPOOL_H_
#define IOBUFFERPOOL_H_
#include <walle/sys/Mutex.h>

class QCIoBufferPool
{
public:
    typedef int RefillReqId;
    enum
    {
        kRefillReqIdUndefined = -1,
        kRefillReqIdRead      = 1
    };

    class Client
    {
    public:
        virtual void Release(
            RefillReqId inReqId,
            int         inBufCount) = 0;
        virtual bool Unregister();
        QCIoBufferPool* GetPoolPtr() const
            { return mPoolPtr; }
        bool IsRegistered()
            { return (mPoolPtr != 0); }
    protected:
        Client();
        virtual ~Client()
            { Client::Unregister(); }
    private:
        QCIoBufferPool* mPoolPtr;
        Client*         mPrevPtr[1];
        Client*         mNextPtr[1];
    friend class QCIoBufferPool;
    template<typename, unsigned int> friend class QCDLListOp;
    };

    class InputIterator
    {
    public:
        virtual char* Get() = 0;
    protected:
        InputIterator()
            {}
        virtual ~InputIterator()
            {}
    };

    class OutputIterator
    {
    public:
        virtual void Put(
            char* inBufPtr) = 0;
    protected:
        OutputIterator()
            {}
        virtual ~OutputIterator()
            {}
    };

    QCIoBufferPool();
    ~QCIoBufferPool();
    int Create(
        int          inPartitionCount,
        int          inPartitionBufferCount,
        int          inBufferSize,
        bool         inLockMemoryFlag);
    void Destroy();
    char* Get(
        RefillReqId inRefillReqId = kRefillReqIdUndefined);
    bool Get(
        OutputIterator& inIt,
        int             inBufCnt,
        RefillReqId     inRefillReqId = kRefillReqIdUndefined);
    void Put(
        char* inBufPtr);
    void Put(
        InputIterator& inIt,
        int            inBufCnt);
    bool Register(
        Client& inClient);
    bool UnRegister(
        Client& inClient);
    int GetBufferSize() const
        { return mBufferSize; }
    int GetFreeBufferCount();

private:
    class Partition;
    Mutex      mMutex;
    Client*    mClientListPtr[1];
    Partition* mPartitionListPtr[1];
    int        mBufferSize;
    int        mFreeCnt;

    bool TryToRefill(
        RefillReqId inReqId,
        int         inBufCnt);
    void PutSelf(
        char* inBufPtr);

public:
    // No copies.
    QCIoBufferPool(
        const QCIoBufferPool& inPool);
    QCIoBufferPool& operator=(
        const QCIoBufferPool& inPool);
};

#endif /* QCIOBUFFERPOOL_H */
