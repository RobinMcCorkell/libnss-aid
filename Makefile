CXXFLAGS=-Wall -fPIC -std=c++11
LDFLAGS=-shared -Wl,-soname,libnss_aid.so.2

all: libnss_aid.so.2

debug: CXXFLAGS += -g
debug: all

libnss_aid.so.2: libnss_aid.o AidLoader.o DataEntry.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o libnss_aid.so.2

install:
	cp libnss_aid.so.2 /lib

