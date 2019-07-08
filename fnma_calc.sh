#!/bin/bash

if [ "$#" -ne "1" ]; then
	echo "USAGE: $0 <int:node-id>"
	exit 0
fi
n=$1

# sdkperf_c -cip=shm://localhost:54321 -stl=job/$n -md
./bin/calculator -c configs/ipc_shmem.properties -s job/$n
