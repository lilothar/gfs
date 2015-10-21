#ifndef COMMON_SHOW_H
#define COMMON_SHOW_H

#include <ostream>

namespace GFS
{
using std::ostream;

template<typename ObjT, typename StreamT>
class ShowObjectT
{
public:
    ShowObjectT(
        const ObjT& inTarget)
        : _target(inTarget)
        {}
    StreamT& Show(
        StreamT& inStream) const
        { return _target.Show(inStream); }
private:
    const ObjT& _target;
};

template<typename ObjT, typename StreamT>
static inline StreamT& operator << (StreamT& inStream,
    const ShowObjectT<ObjT, StreamT>& inShow)
{ 
    return inShow.Show(inStream); 
}

template<typename ObjT, typename StreamT>
    static inline ShowObjectT<ObjT, StreamT>
Show(const ObjT& inObj, StreamT* /* inNullArgPtr */)
{ return ShowObjectT<ObjT, StreamT>(inObj); }

template<typename ObjT>
    static inline ShowObjectT<ObjT, ostream>
Show(const ObjT& inObj)
{ return ShowObjectT<ObjT, ostream>(inObj); }

}

#endif 
