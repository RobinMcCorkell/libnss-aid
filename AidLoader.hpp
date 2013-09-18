// AidLoader.hpp
// AidLoader header file

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

#ifndef HPP_AIDLOADER
#define HPP_AIDLOADER

#include <string>
#include <vector>
#include <boost/program_options.hpp>

#include "DataEntry.hpp"

class AidLoader
{
public:
	AidLoader();
	AidLoader(const std::string& configFilename);
	const std::vector<DataEntry>& getDb() const;
	const boost::program_options::variables_map& getConfig() const;

private:
	std::vector<DataEntry> vDb;
	boost::program_options::variables_map mConfig;
};

#endif
