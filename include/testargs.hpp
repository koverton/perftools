#ifndef TESTARGS_HPP
#define TESTARGS_HPP
#include <sys/stat.h>
#include <iostream>

typedef enum desttype
{
    TOPIC, QUEUE, DURSUB
} desttype;

typedef struct test_def 
{
	std::string props;
	std::string dest;
	desttype    dtype;
	int         rate;
	int         seconds;
} test_def;

void 
usage(const std::string& program, const std::string& message)
{
	std::cerr << std::endl << message << std::endl
		  << "USAGE: " << program
		  << " <conn-props-file> [-q <queue> or -t <topic>] -r <rate> -d <duration-secs>" 
		  << std::endl;
	exit( 0 );
}

void 
parse_args ( int c, char *v[], test_def* test)
{
	if (c < 8) usage( v[0], "Incorrect number of arguments" );
	int set = 0, i = 1;
	test->props += v[i++];
	struct stat buffer;   
	if ( stat(test->props.c_str(), &buffer) != 0 )
		usage( v[0], std::string("Can't open ")+test->props );

	for( ; i < c; i++) {
		if ( !strncmp(v[i], "-q", 2) ) {
			test->dest += v[++i];
			test->dtype = QUEUE;
			set++;
		}
		else if ( !strncmp(v[i], "-t", 2) ) {
			test->dest += v[++i];
			test->dtype = TOPIC;
			set++;
		}
		else if ( !strncmp(v[i], "-r", 2) ) {
			test->rate += atoi( v[++i] );
			set++;
		}
		else if ( !strncmp(v[i], "-d", 2) ) {
			test->seconds += atoi( v[++i] );
			set++;
		}
		else {
			usage( v[0], std::string("Unknown argument ")+v[i] );
		}
	}
}

#endif// TESTARGS_HPP
