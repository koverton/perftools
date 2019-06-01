#ifndef EVENT_LOG_READER
#define EVENT_LOG_READER
#include "event_record.hpp"

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>

class event_log_reader {
public:
    event_log_reader(const std::string& fnames_base);

    inline int count() const { return index_[0]; }

    record read_next();

    record read(int offset);

    void dump_index(bool brief = true);

private:
    template <typename T>
    const char* read_scalar(const char* pos, T& t) 
    {
        t = *(reinterpret_cast<const T*>(pos)); pos += sizeof(T);
        return pos;
    }

    const char* read_tstamp (const char* pos, record& l);
    const char* read_topic  (const char* pos, record& l);
    const char* read_payload(const char* pos, record& l);

    inline const int* front() const { return index_+1; }

    boost::iostreams::mapped_file_source ndx_;
    boost::iostreams::mapped_file_source log_;
    const int* index_; 
    const char* base_;
    int next_, nextoff_;
};

#endif // EVENT_LOG_READER
