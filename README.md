# Solace perftools

Just a general workspace in which I prototype various tools for high-performance tests or eventing.


IPC-based grid compute example with the following applications:
1. `bin/feedHandler`: this connects to an orchestrator process and sends it input messages. For this example the input messages just include a timestamp for calculating latency distributions.
2. `bin/orchestrator`: this process binds to a socket waiting for connections from a feedHandler data source and calculator destination processes. When it receives messages, it chooses one of the calculators to route the messages to in a round-robin fashion while calculating latency.
3. `bin/calculator`: this process represents the end calculation nodes that receive messages from the orchestrator and process them. For now, they just calculate latency from the feedHandler to calc nodes.

To build this example under 64-bit Linux, you must have BOOST libraries installed in your dev environment and download the [Solace C-language SDK](https://products.solace.com/download/C_API_LINUX64). Untar/unzip the CCSMP library and add it's `[CCSMPDIR]/lib` directory to your LD_LIBRARY_PATH. Update the `Makefile` variable `SOLCLIENT_DIR` to point to this directory in your local system and try running:

```
make bin/feedHandler bin/orchestrator bin/calculator
```

If all goes well, the executables should appear in `bin/`. There are example shell scripts to run them with same basic pre-selected arguments. You should start them in this order:
1. `./fnma_orch.sh`
2. `./fnma_calc.sh 1`
3. `./fnma_calc.sh 2`
4. `./fnma_fh.sh` # begins sending input messages


Additional examples:
* `bin/perfADTest`: a high-performance C program to test event latency while varying event rate, size, etc.
* `bin/msgTrace`: a high-performance C program to listen for events and dump them to a memory-mapped file; related programs are `bin/logRead` and `bin/dumpIndex`.


