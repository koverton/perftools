#ifndef PERF_STATS_HPP
#define PERF_STATS_HPP
#include "data_types.hpp"
#include <vector>
#include <iostream>

typedef std::vector<UINT64> latbuckets;

class perf_stats
{
public:
        perf_stats( UINT64 nbuckets, UINT64 bucket_width )
                : _buckets(nbuckets), _width(bucket_width) {}

        void add(UINT64 latency) {
		_count++;
		UINT64 bucket = latency/_width;
		if (bucket > _buckets.size())
			_overflow++;
		else
                	_buckets[ (latency/_width) ]++;
        }

	UINT64 count() const { return _count; }

        void debug() const {
                int i = 0;
                for(latbuckets::const_iterator it = _buckets.begin(); it != _buckets.end(); ++it) {
			if (i >= _count) break;
                        std::cout << i++ << " : " << *it << std::endl;
        	}
	}

        double get_pct(double perc) const;

private:
        latbuckets _buckets;
        UINT32     _width;
	UINT64     _count;
	UINT64     _overflow;
};


#endif// PERF_STATS_HPP
