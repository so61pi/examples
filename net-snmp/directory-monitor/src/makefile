CC=gcc
CFLAGS=-Wall
OBJS=monitor-agent.o Directory.o DirectoryStateNotification.o NumDirectories.o \
     NumFiles.o Utility.o GlobalSettings.o DirectoryContentTable.o \
     DirectoryContentTable_data_access.o DirectoryContentTable_data_get.o \
     DirectoryContentTable_data_set.o DirectoryContentTable_interface.o
LIBS=-lnetsnmp -lnetsnmpagent


%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

monitor-agent.out: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f monitor-agent.out
