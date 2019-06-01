#!/bin/bash
cd `dirname $0`/..

# taskset -c 9,11 \
	sdkperf_c -cip=listen://localhost:54321 -ptl=a/b/c,d/e/f -mn=10000000000 -mr=1000000 -msa=1000
