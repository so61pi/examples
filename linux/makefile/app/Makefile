PREFIX		?= /usr/local

CC			?= gcc
CXX			?= g++
INSTALL		?= install
RM			?= rm

CFLAGS		?= -Wall -Wextra -Werror -O3
CXXFLAGS	?= -Wall -Wextra -Werror -O3

SRC			= $(wildcard *.c)
OBJ			= $(SRC:.c=.o)
BIN			= example


%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $^

%.o:%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $^

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: install
install: $(BIN)
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -m 0755 $(BIN) $(DESTDIR)$(PREFIX)/bin

.PHONY: clean
clean:
	$(RM) $(BIN) $(OBJ)
