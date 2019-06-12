#include "event_log_writer.hpp"
#include  <iomanip>

#define HI(ulong) static_cast<UINT32>(ulong>>32)
#define LO(ulong) static_cast<UINT32>(0x00000000FFFFFFFF & ulong)

boost::iostreams::mapped_file_params event_log_params(const std::string& fname, boost::iostreams::stream_offset size)
{
    std::cout << "Creating memory-mapped file for " << fname << std::endl;
    boost::iostreams::mapped_file_params params( fname );
    params.flags = boost::iostreams::mapped_file::mapmode::readwrite;
    params.new_file_size = size;
    return params;
}

event_log_writer::event_log_writer( const std::string& fnames_base, int log_size ) 
    : count_(0)
    , written_(0)
    , basename_(fnames_base)
    , filesz_(log_size)
    , ndx_( event_log_params(fnames_base+INDEXFILE_EXT, log_size/4) )
    , out_( event_log_params(fnames_base+LOGFILE_EXT  , log_size) )
    {
        count_ = index_ = (int*)(*ndx_).data();
        *(index_++) = 0; // set the count to zero and increment to the next slot
    }


int event_log_writer::write(const record& l) 
{
    int totalsz = l.totalsz();
    if ( !has_space(totalsz) ) 
        return 0;
    if ( bad_state() )
        return -1;
    *(index_++) = totalsz; // sets the index entry to the record size
    int written = write_scalar( &(l.tstamp_) ); 
    written    += write_scalar( &(l.tlen_) );
    written    += write_str( l.topic_, l.tlen_ );
    written    += write_scalar( &(l.plen_) );
    written    += write_str( l.payload_, l.plen_ );
    written_   += written;
    (*count_)++; // increment the index record-count
    return written;
}

int event_log_writer::write(const char* data, int length) 
{
    if ( !has_space(length) ) 
        return 0;
    if ( bad_state() )
        return -1;
    *(index_++) = length; // sets the index entry to the record size
    int written = write_str( data, length );
    written_   += written;
    (*count_)++; // increment the index record-count
    return written;
}

int event_log_writer::write_str(void* str, int slen) 
{
    return write_str( reinterpret_cast<const char*>(str), slen );
}

int event_log_writer::write_str(const char* str, int slen) 
{
    out_.write( str, slen )
        .flush();
    if ( out_.good() )
        return slen;
    return 0;
}


