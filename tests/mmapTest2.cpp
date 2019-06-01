#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <vector>
#include <iostream>

namespace bio = boost::iostreams;

#define FNAME "ints.dat"
int main() {
    using namespace std;

    int size = 100;

    bio::mapped_file_params params( FNAME );
    params.new_file_size = size*16;
    params.flags         = bio::mapped_file::mapmode::readwrite;
    bio::mapped_file_sink out(params);

    void* vf = (void*)out.data();
    int* ints = (int*)vf;
    for(int i = 0; i < 10; ++i) 
	    ints[i] = i;

    bio::mapped_file_source in;
    in.open( FNAME );
    vf = (void*)in.data();
    int* ro = (int*)vf;
    for(int i = 0; i < 10; ++i) 
	    std::cout << ro[i] << std::endl;

}

