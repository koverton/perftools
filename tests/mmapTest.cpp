#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <vector>
#include <iostream>

namespace bio = boost::iostreams;

typedef struct foo 
{
	int    x_;
	double y_; 
} foo;

void show(const foo& f) { std::cout << "int: " << f.x_ << " float: " << f.y_ << std::endl; }

int main() {
    using namespace std;

    bio::mapped_file_params params( "text.txt" );
    params.new_file_size = 1000;
    params.flags         = bio::mapped_file::mapmode::readwrite;
    bio::stream<bio::mapped_file_sink> out(params);

    foo f;
    f.x_ = 12345;
    f.y_ = 1.2345;
    show( f );
    void* vf = (void*) &f;
    out.write( (const char*) vf, sizeof(f) );
    out.flush();
    out.close();

    bio::mapped_file_source in;
    in.open( "text.txt" );
    vf = (void*)in.data();
    const foo* fi = (const foo*)vf;

    show( *fi );
}

