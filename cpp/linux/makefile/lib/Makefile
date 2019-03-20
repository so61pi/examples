PREFIX		?= /usr/local

MAJOR		= 0
MINOR		= 1
VERSION		= $(MAJOR).$(MINOR)

LIBSO		= libexample.so
LIBSOM		= $(LIBSO).$(MAJOR)
LIBSOV		= $(LIBSO).$(VERSION)

CC			?= gcc
CXX			?= g++
INSTALL		?= install
RM			?= rm
LN			?= ln

CFLAGS		?= -Wall -Wextra -Werror -O3
CXXFLAGS	?= -Wall -Wextra -Werror -O3

SRC			= $(wildcard *.c)
OBJ			= $(SRC:.c=.o)


%.o:%.c
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -fPIC -o $@ $<

$(LIBSOV): $(OBJ)
	$(CC) -shared -Wl,-soname,$(LIBSOM) -o $@ $^ $(LDFLAGS)

.PHONY: install
install: $(LIBSOV)
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/lib
	$(INSTALL) -m 0644 $(LIBSOV) $(DESTDIR)$(PREFIX)/lib
	cd $(DESTDIR)$(PREFIX)/lib; $(LN) -s $(LIBSOV) $(LIBSO)

	$(INSTALL) -d $(DESTDIR)$(PREFIX)/include
	$(INSTALL) -m 0644 libexample.h $(DESTDIR)$(PREFIX)/include

.PHONY: clean
clean:
	$(RM) $(LIBSOV) $(OBJ)
