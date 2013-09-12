// AidLoader.cpp
// AidLoader implementation

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

#include "AidLoader.hpp"

AidLoader::AidLoader() : AidLoader("/etc/libnss-aid.conf")
{
}

AidLoader::AidLoader(std::string configFilename)
{
	//ignore configFilename for now
	
	//Generate vDb entries
	{
		vDb.push_back( { "aid_system", 1000 } );
		vDb.push_back( { "aid_radio", 1001 } );
		vDb.push_back( { "aid_bluetooth", 1002 } );
	}
}

const std::vector<DataEntry>& AidLoader::getDb() const
{
	return vDb;
}
