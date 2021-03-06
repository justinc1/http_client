CFLAGS = -Wall -Wextra -I.
CXXFLAGS = $(CFLAGS)

GCC=gcc $(CFLAGS) -ggdb -pthread
GCCSO=$(GCC) -fPIC -shared
CPP=g++ $(CXXFLAGS) -ggdb -pthread
CPPSO=$(CPP) -fPIC -shared

EXEC=http_client http_client.so

all: $(EXEC)

.PHONY: module
module: all

clean:
	/bin/rm -f $(EXEC)

http_client: http_client.c
	$(CPP) -o $@ $<
http_client.so: http_client.c
	$(CPPSO) -o $@ $<
