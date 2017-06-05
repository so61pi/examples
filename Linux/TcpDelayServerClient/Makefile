CXX			?= g++
RM			?= rm

CXXFLAGS	?= -Wall -Wextra -O3 -std=c++11

SERVERSRC	= server.cpp
SERVEROBJ	= $(SERVERSRC:.cpp=.o)
SERVERBIN	= server

CLIENTSRC	= client.cpp
CLIENTOBJ	= $(CLIENTSRC:.cpp=.o)
CLIENTBIN	= client


%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

all: $(SERVERBIN) $(CLIENTBIN)

$(SERVERBIN): $(SERVEROBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(CLIENTBIN): $(CLIENTOBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(SERVERBIN) $(CLIENTBIN) $(SERVEROBJ) $(CLIENTOBJ)
