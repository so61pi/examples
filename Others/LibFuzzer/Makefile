CXX			= clang++-6.0
RM			?= rm

COVERAGE	= -fprofile-instr-generate -fcoverage-mapping
SANITIZER	= -fsanitize=fuzzer -fsanitize-coverage=trace-pc-guard -fsanitize-coverage=trace-cmp

# coverage seems to work only when its flags go before sanitizer flags
CXXFLAGS	= -Wall -Wextra -g3 -O3 $(COVERAGE) $(SANITIZER)
LDFLAGS		= $(COVERAGE) $(SANITIZER)

SRC			= $(wildcard *.cpp)
OBJ			= $(SRC:.cpp=.o)
BIN			= test


# RUNS		= -1
RUNS		= 50000000


PROFRAWFILE	= $(BIN).profraw
PROFDATAFILE= $(BIN).profdata
REPORTFILE	= report.html


.PHONY: all
all: $(BIN)


.PHONY: run
run: $(BIN)
	LLVM_PROFILE_FILE=$(PROFRAWFILE) ./$(BIN) -runs=$(RUNS) -use_value_profile=1 corpus
	llvm-profdata-6.0 merge -sparse $(PROFRAWFILE) -o $(PROFDATAFILE)
	llvm-cov-6.0 show $(BIN) -format=html -instr-profile=$(PROFDATAFILE) > $(REPORTFILE)
	xdg-open $(REPORTFILE)


.PHONY: merge
merge:
	rm -rf new-corpus && mkdir new-corpus
	./$(BIN) -merge=1 new-corpus corpus
	rm -rf corpus
	mv new-corpus corpus


%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^


$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)


.PHONY: clean
clean:
	$(RM) $(BIN) $(OBJ) $(PROFRAWFILE) $(PROFDATAFILE) $(REPORTFILE)
