#ifndef METANODE_INODE_H_
#define METANODE_INODE_H_
#include <stdint.h>

namespace GFS {

class Inode{
public:
    Inode(){}
public:
    int64_t _iid;
    int32_t _itype;
    int32_t _ctime;
    int32_t _mtime;
    int32_t _atime;
    

};


}
#endif
