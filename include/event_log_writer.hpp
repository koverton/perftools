#ifndef EVENT_LOG_WRITER
#define EVENT_LOG_WRITER
#include "event_record.hpp"
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>

class event_log_writer {
public:
    event_log_writer( const std::string& fnames_base, int log_size );

    int write(const record& l);
    int write(const char* data, int length);

    bool has_space(int size) const {
        return size < remaining();
    }

    int remaining() const {
        return filesz_ - written_;
    }

    int count() const { return *count_; }

    bool bad_state() {
        return out_.bad() || out_.fail() || out_.eof();
    }
    int size() const { return filesz_; }
    const std::string& basename() const { return basename_; }

private:
    template <typename T>
    int write_scalar(const T* t) {
        out_.write( reinterpret_cast<const char*>(t), sizeof(T) )
            .flush();
        if ( out_.good() )
            return sizeof(T);
        return 0;
    }
    int write_str(void* str, int slen);
    int write_str(const char* str, int slen);

    boost::iostreams::stream<boost::iostreams::mapped_file_sink> out_;
    boost::iostreams::stream<boost::iostreams::mapped_file_sink> ndx_;
    int* index_; // address in the mmapped indexfile
    int* count_; // address in the mmapped indexfile
    int  written_; // total bytes written so far
    int  filesz_;  // total size of the file
    const std::string basename_;
};

#endif // EVENT_LOG_WRITER
