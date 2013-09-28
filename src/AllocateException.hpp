// AllocateException.hpp
// AllocateException header file

// Copyright (C) 2013 Robin McCorkell
// This file is part of libnss-aid.
//
// libnss-aid is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libnss-aid is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with libnss-aid.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HPP_ALLOCATEEXCEPTION
#define HPP_ALLOCATEEXCEPTION

#include <string>
#include <stdexcept>

class AllocateException : public std::runtime_error
{
public:
	AllocateException(const std::string& msg) : runtime_error(msg) {};
};

#endif
