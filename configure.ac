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
# along with libnss-aid.  If not, see <http://www.gnu.org/licenses/>.

# Initialise autoconf
AC_INIT([libnss-aid], [1.0], [rmccorkell@karoshi.org.uk])
AC_PREREQ([2.61])

# Configuration
AC_CONFIG_SRCDIR([src/libnss_aid.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_PREFIX_DEFAULT()
AC_DISABLE_STATIC

# Initialise automake and libtool
AM_INIT_AUTOMAKE([subdir-objects])
LT_INIT

# Checks
AC_TYPE_UID_T
AC_TYPE_SIZE_T

AC_FUNC_REALLOC

# C header checks
AC_CHECK_HEADERS([sys/types.h nss.h pwd.h grp.h])

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MKDIR_P

# Boost libraries
BOOST_REQUIRE
BOOST_PROGRAM_OPTIONS

# C++11 support
AX_CXX_COMPILE_STDCXX_11

# Additional C++ support
AX_CXX_HAVE_STD
AX_CXX_HAVE_STL
AX_CXX_NAMESPACES
AX_CXX_TEMPLATES
AX_CXX_BOOL
AX_CXX_EXCEPTIONS

# Path to configuration file
AC_ARG_WITH(conf-file,
	[AS_HELP_STRING([--with-conf-file=PATH],
		[name of configuration file @<:@default: /etc/libnss-aid.conf@:>@])],
	[ conf_path="$withval" ],
	[ conf_path="libnss-aid.conf" ])
AC_DEFINE_UNQUOTED([CONF_PATH], ["$conf_path"], [Path to configuration file])
AC_SUBST([conf_path])

AC_OUTPUT

