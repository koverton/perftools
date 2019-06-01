#!/bin/bash
cd `dirname $0`/..

# taskset -c 12,14 \
	./bin/msgTrace configs/test1.properties data/events 1000000000 a/b/c d/e/f
