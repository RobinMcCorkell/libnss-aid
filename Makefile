# Copyright (C) 2013 Robin McCorkell
# This file is part of libnss-aid.
#
# libnss-aid is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libnss-aid is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libnss-aid.  If not, see <http:#www.gnu.org/licenses/>.

CXXFLAGS=-Wall -fPIC -std=c++11
LDFLAGS=-shared -Wl,-soname,libnss_aid.so.2 -lboost_program_options

all: libnss_aid.so.2

debug: CXXFLAGS += -g
debug: all

libnss_aid.so.2: libnss_aid.o AidLoader.o DataEntry.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o libnss_aid.so.2

install:
	install libnss_aid.so.2 /lib
	cp -i libnss-aid.conf /etc

uninstall:
	rm -f /lib/libnss_aid.so.2

purge: uninstall
	rm -f /etc/libnss-aid.conf
