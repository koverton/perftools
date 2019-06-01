#ifndef EVENT_RECORD_HPP
#define EVENT_RECORD_HPP
#include "clock.hpp"
#include "data_types.hpp"
#include <iostream>
#include <iomanip>

#define LOGFILE_EXT ".dat"
#define INDEXFILE_EXT ".ndx"

typedef struct record 
{
    record() 
        : topic_(0)
        , tlen_(0)
        , payload_(0)
        , plen_(0) 
        , tstamp_(0)
        {}
    record(const char* topic, void* payload, int length)
        : topic_(topic)
        , tlen_(1+strlen(topic))
        , payload_(payload)
        , plen_(length)
        , tstamp_(getTimeInMs())
        { }

    inline int totalsz() const {
        return sizeof(tstamp_) + sizeof(tlen_) + tlen_ + sizeof(plen_) + plen_;
    }

    void show() const {
        std::cout << "time: 0x" << std::setfill('0') << std::setw(16) << std::hex << tstamp_ << std::endl
                  << "topic: " << topic_ << std::endl
                  << "msg: "   << static_cast<const char*>(payload_) << std::endl;
    }

    const char* topic_;
    int         tlen_;
    void*       payload_; 
    int         plen_;
    UINT64      tstamp_;
} record;

#endif // EVENT_RECORD_HPP
