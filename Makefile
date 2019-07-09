BIN     = ./bin
SRC     = ./src
SOLCLIENT_DIR ?= ~/solclient/c/7.4.0.12
INCDIRS = include $(SOLCLIENT_DIR)/include  # Add boost here if necessary
SYMS    = PROVIDE_LOG_UTILITIES SOLCLIENT_CONST_PROPERTIES _LINUX_X86_64
DEBUG   = # -g

LIBDIRS = $(SOLCLIENT_DIR)/lib .  # Add boost here if necessary
LIBS    = solclient pthread boost_iostreams boost_program_options

CXXFLAGS= $(foreach d, $(INCDIRS), -I$d) $(foreach s, $(SYMS), -D$s) $(DEBUG) -std=c++11 -m64 -Woverloaded-virtual -fmessage-length=0
LDFLAGS = $(foreach d, $(LIBDIRS), -L$d) $(foreach l, $(LIBS), -l$l)

SOL_SRC = $(wildcard $(SRC)/*.cpp)

ALL     = $(BIN)/clockTest $(BIN)/statsTest $(BIN)/runTest $(BIN)/perfADTest grid trace

all: $(ALL)

grid: $(BIN)/feedHandler $(BIN)/orchestrator $(BIN)/calculator

trace: $(BIN)/msgTrace $(BIN)/logWrite $(BIN)/logRead $(BIN)/dumpIndex

##
## Individual module tests (quasi unit tests)
##
$(BIN)/clockTest: tests/clockTest.cpp src/clock.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/statsTest: tests/statsTest.cpp src/perf_stats.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/runTest: tests/runTest.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/hdrTest: tests/hdrTest.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^


##
## Latency tester
##
$(BIN)/perfADTest: tests/perfADTest.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

## 
## Msg Trace log apps
##
$(BIN)/logWrite: tests/logWrite.cpp src/event_log_writer.cpp src/clock.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/logRead: tests/logRead.cpp src/event_log_reader.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/dumpIndex: tests/dumpIndex.cpp src/event_log_reader.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/msgTrace: tests/msgTrace.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^


##
## FNMA grid demo processes
##
$(BIN)/feedHandler: tests/feedHandler.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/orchestrator: tests/orchestrator.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/calculator: tests/calculator.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^


clean:
	$(RM) $(ALL) data/*.*
