#ifndef COMMON_STD_ALLOCATOR_H_
#define COMMON_STD_ALLOCATOR_H_


#include <ext/pool_allocator.h>
#define GFS_STD_POOL_ALLOCATOR_T      __gnu_cxx::__pool_alloc
#define GFS_STD_FAST_POOL_ALLOCATOR_T __gnu_cxx::__pool_alloc

namespace GFS
{

template <
    typename T,
    typename ALLOCATOR = GFS_STD_POOL_ALLOCATOR_T<T>
>
class StdAllocator : public ALLOCATOR::template rebind<T>::other
{
private:
    typedef typename ALLOCATOR::template rebind<T>::other MySuper;
public:
    typedef typename MySuper::pointer   pointer;
    typedef typename MySuper::size_type size_type;

    pointer allocate(
        size_type inCount)
    {
        return (inCount == 0 ? (pointer)"" : MySuper::allocate(inCount));
    }
    void deallocate(
        pointer   inPtr,
        size_type inCount)
    {
        if (inCount != 0) {
            MySuper::deallocate(inPtr, inCount);
        }
    }
    template <typename U>
    struct rebind
    {
        typedef StdAllocator<U, ALLOCATOR> other;
    };
    StdAllocator()
        : MySuper()
        {}
    StdAllocator(
        const StdAllocator& inAlloc)
        : MySuper(inAlloc)
        {}
    template<typename Tp, typename Ap>
    StdAllocator(
        const StdAllocator<Tp, Ap>& inAlloc)
        : MySuper(inAlloc)
        {}
    ~StdAllocator()
        {}
};

template <
    typename T,
    typename ALLOCATOR = GFS_STD_FAST_POOL_ALLOCATOR_T<T>
>
class StdFastAllocator : public StdAllocator<T, ALLOCATOR>
{
private:
    typedef StdAllocator<T, ALLOCATOR> MySuper;
public:
    template <typename U>
    struct rebind
    {
        typedef StdFastAllocator<U, ALLOCATOR> other;
    };
    StdFastAllocator()
        : MySuper()
        {}
    StdFastAllocator(
        const StdFastAllocator& inAlloc)
        : MySuper(inAlloc)
        {}
    template<typename Tp, typename Ap>
    StdFastAllocator(
        const StdFastAllocator<Tp, Ap>& inAlloc)
        : MySuper(inAlloc)
        {}
    ~StdFastAllocator()
        {}
};

} 

#endif 
