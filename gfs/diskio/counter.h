#ifndef DISKIO_COUNTER_H_
#define DISKIO_COUNTER_H_
#include <walle/sys/Atomic.h>
#include <string>
#include <stdint.h>
#include <ostream>
#include <map>
#include <algorithm>

namespace GFS {
using std::ostream;
using std::string;
using std::map;
using walle::sys::AtomicInt64;
using std::for_each;
class Counter {
public:
    Counter():_name(""){}
    Counter(const string &inName):_name(inName){}
    virtual ~Counter() {}

    void show(ostream &os)
    {
        os<<_name<<": "<<_count.get()
          <<", "
          <<_timeCost.get()*1e-6
          <<"\r\n";
    }

    void updateCount(int64_t n)
    {
        _count.getAndAdd(n);
    }

    void updateTime(int64_t usecCost)
    {
        _timeCost.getAndAdd(usecCost);
    }

    void reset()
    {
        _timeCost.getAndSet(0);
        _count.getAndSet(0);
    }

    int64_t getCount()
    {
        return _count.get();
    }

    int64_t getTime()
    {
        return _timeCost.get();
    }

    const string& getName() const 
    {
        return _name;
    }

private:
    string       _name;    
    AtomicInt64  _count;
    AtomicInt64  _timeCost;
};

class CounterManager{
public:
    typedef map<string, Counter* > CounterMap;
public:
    CounterManager():_counters();{}
    virtual ~CounterManager(){}

    void addCounter(Counter* c) 
    {
        assert(c);
        _counters[c->getName()] = c;
    }

    void removeCounter(Counter * c) 
    {
        assert(c);
        CounterMap::iterator itr = _counters.find(c->getName());
        if(itr != _counters.end() && itr->second ==c) {
            _counters.erase(itr);
        }
        
    }

    Counter * getCounter(const string & name) 
    {
        CounterMap::iterator itr = _counters.find(name);  
        return (itr == _counters.end()?0:itr->second);
    }
    void show(ostream &os)
    {
         if (_counters.empty()) {
            os << "\r\n";      
            return;
        }
        for_each(_counters.begin(), _counters.end(), _counters(os));
    }
private:
    CounterMap _counters;

};
}
#endif