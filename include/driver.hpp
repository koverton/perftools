#ifndef DRIVER_HPP
#define DRIVER_HPP
#include "data_types.hpp"
#include "clock.hpp"

template <typename context>
class driver 
{
public:
    typedef void (*unit_fn)(context* ctx_p);

    void run(int rate, int seconds, unit_fn fn, context* ctx_p) {
        long double interval = (long double)1000000.0 / (long double) rate;
        UINT64 count = rate * seconds;
        UINT64 target_time = getTimeInUs() + interval;
        for(long i = 0; i < count; i++) {
            cycle( fn, target_time, ctx_p );
            target_time += (UINT64) interval;
        }
    }

private:
    void cycle(unit_fn fn, UINT64 target_time, context* ctx_p) {
        (*fn)( ctx_p );
        UINT64 delta  = target_time - getTimeInUs();
        if ( delta > 0 ) 
            sleepInUs ( (int)(delta) );
    }


};

#endif // DRIVER_HPP
