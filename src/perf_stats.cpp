#include "perf_stats.hpp"
#include <numeric>

bool iswhole(double d) {return static_cast<int> (d == d ? true: false); }


double perf_stats::get_pct(double perc) const
{
        UINT64 total_items = std::accumulate(_buckets.begin(), _buckets.end(), 0);
        UINT64 max_items   = perc * total_items;

        UINT64 this_latency = 0;
        UINT64 num_items = 0;
        for(latbuckets::const_iterator it = _buckets.begin(); it != _buckets.end(); ++it) {
                UINT64 count = *it;
                num_items += count;
                this_latency += _width;
                if (num_items >= max_items)
                        return this_latency;
        }
        return this_latency;
}
