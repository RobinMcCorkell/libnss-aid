// DataEntry.cpp
// DataEntry operator overloads

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

#include "DataEntry.hpp"

bool operator==(const DataEntry& a, const uid_t& b)
{
	return a.id == b;
}
bool operator==(const uid_t& a, const DataEntry& b)
{
	return a == b.id;
}

bool operator==(const DataEntry& a, const std::string& b)
{
	return a.name == b;
}
bool operator==(const std::string& a, const DataEntry& b)
{
	return a == b.name;
}

