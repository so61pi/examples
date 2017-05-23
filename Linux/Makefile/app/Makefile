PREFIX		?= /usr/local

CC			?= gcc
CXX			?= g++
CFLAGS		?= -Wall -Wextra -Werror -O3
CXXFLAGS	?= -Wall -Wextra -Werror -O3

SRC			= $(wildcard *.c)
OBJ			= $(SRC:.c=.o)
BIN			= example


%.o:%c
	$(CC) -c $(CFLAGS) -o $@ $^

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: install
install: $(BIN)
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 0755 $(BIN) $(DESTDIR)$(PREFIX)/bin

.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ)
