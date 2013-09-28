## Process this file with automake to produce Makefile.in

## Copyright (C) 2013 Robin McCorkell
## This file is part of libnss-aid.
##
## libnss-aid is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## libnss-aid is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with libnss-aid.  If not, see <http://www.gnu.org/licenses/>.

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libnss_aid.la
libnss_aid_la_SOURCES = src/libnss_aid.cpp src/AidLoader.cpp src/DataEntry.cpp

libnss_aid_la_CPPFLAGS = $(BOOST_CPPFLAGS)
libnss_aid_la_LDFLAGS = -version-info 2:0:0 $(BOOST_PROGRAM_OPTIONS_LDFLAGS)
libnss_aid_la_LIBADD = $(BOOST_PROGRAM_OPTIONS_LIBS)

# Configuration file
conf_path = @conf_path@

install-data-local: install-libnss-aid_conf
uninstall-local: uninstall-libnss-aid_conf

install-libnss-aid_conf:
	@if [ -f $(DESTDIR)$(conf_path) ]; then \
	 echo "WARNING: $(DESTDIR)$(conf_path) already exists, not overwriting"; \
	else \
	 $(MKDIR_P) `dirname $(DESTDIR)$(conf_path)`; \
	 $(INSTALL) -m 644 $(srcdir)/libnss-aid.conf $(DESTDIR)$(conf_path); \
	fi

uninstall-libnss-aid_conf:
	-rm -f $(DESTDIR)$(conf_path)
