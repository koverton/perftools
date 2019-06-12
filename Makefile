BIN     = ./bin
SRC     = ./src
SOLCLIENT_DIR ?= ~/solclient/c/7.4.0.12
INCDIRS = include $(SOLCLIENT_DIR)/include
SYMS    = PROVIDE_LOG_UTILITIES SOLCLIENT_CONST_PROPERTIES _LINUX_X86_64
DEBUG   =

LIBDIRS = $(SOLCLIENT_DIR)/lib .
LIBS    = solclient pthread boost_iostreams

CXXFLAGS= $(foreach d, $(INCDIRS), -I$d) $(foreach s, $(SYMS), -D$s) -m64 $(DEBUG) -m64 -Woverloaded-virtual -fmessage-length=0
LDFLAGS = $(foreach d, $(LIBDIRS), -L$d) $(foreach l, $(LIBS), -l$l)

SOL_SRC = $(wildcard $(SRC)/*.cpp)

ALL     = $(BIN)/msgTrace $(BIN)/perfADTest $(BIN)/clockTest $(BIN)/statsTest $(BIN)/runTest $(BIN)/logWrite $(BIN)/logRead $(BIN)/dumpIndex

all: $(ALL)

$(BIN)/msgTrace: tests/msgTrace.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/perfADTest: tests/perfADTest.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/clockTest: tests/clockTest.cpp src/clock.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/statsTest: tests/statsTest.cpp src/perf_stats.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/runTest: tests/runTest.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/hdrTest: tests/hdrTest.cpp $(SOL_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/logWrite: tests/logWrite.cpp src/event_log_writer.cpp src/clock.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/logRead: tests/logRead.cpp src/event_log_reader.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(BIN)/dumpIndex: tests/dumpIndex.cpp src/event_log_reader.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^


clean:
	$(RM) $(ALL) data/*.*
