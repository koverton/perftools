#include "event_log_reader.hpp"
#include "solclient/solClient.h"

#include <numeric>

// Concat two UINT32s into one UINT64
#define CAT(i1,i2) (UINT64)i1<<32 | (UINT64)i2

event_log_reader::event_log_reader(const std::string& fnames_base)
    : ndx_(fnames_base+INDEXFILE_EXT)
    , log_(fnames_base+LOGFILE_EXT)
    , next_(1)
    , nextoff_(0)
    {
        index_ = reinterpret_cast<const int*> ( ndx_.data() );
        base_  = reinterpret_cast<const char*>( log_.data() );
    }


solClient_opaqueMsg_pt event_log_reader::read_next_msg() 
{
    if ( next_ > count() ) return NULL;
    int size = index_[next_++];
    solClient_opaqueMsg_pt m = read_msg( nextoff_, size );
    nextoff_ += size;
    return m;
}
solClient_opaqueMsg_pt event_log_reader::read_msg(int offset, int size) 
{
    solClient_bufInfo_t smfbuf;
    smfbuf.buf_p = (void*)(base_ + offset);
    smfbuf.bufSize = size;
    solClient_opaqueMsg_pt msg_p;
    solClient_msg_decodeFromSmf( &smfbuf, &msg_p );
    return msg_p;
}

record event_log_reader::read_next() 
{
    if ( next_ > count() ) return record();
    record l = read( nextoff_ );
    nextoff_ += index_[next_++];
    return l;
}
record event_log_reader::read(int offset) 
{
    record l;
    const char* pos = base_ + offset;
    pos = read_tstamp( pos, l );
    pos = read_topic( pos, l );
    pos = read_payload( pos, l );
    return l;
}

const char* event_log_reader::read_tstamp(const char* pos, record& l) 
{
    UINT32 hi, lo;
    pos = read_scalar( pos, hi );
    pos = read_scalar( pos, lo );
    l.tstamp_ = CAT( hi, lo );
    return pos;
}

const char* event_log_reader::read_topic(const char* pos, record& l) 
{
    pos = read_scalar( pos, l.tlen_ );
    l.topic_ = pos;        pos += l.tlen_;
    return pos;
}

const char* event_log_reader::read_payload(const char* pos, record& l) 
{
    pos = read_scalar( pos, l.plen_ );
    l.payload_ = static_cast<void*>( const_cast<char*>(pos) );
    return pos;
}

void event_log_reader::dump_index(bool brief) 
{
    std::cout << "\ttotal entries: " << count() << std::endl;
    int totalsz = std::accumulate( front(), front()+count(), 0 );
    std::cout << "\ttotal storage: " << totalsz << std::endl;
    if ( !brief ) {
        for( int i = 1; i <= index_[0]; i++)
            std::cout << index_[i] << std::endl;
    }
}

