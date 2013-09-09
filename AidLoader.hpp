// AidLoader.hpp
// part of libnss-aid
//
// AidLoader header file
//
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

#ifndef HPP_AIDLOADER
#define HPP_AIDLOADER

#include <string>
#include <vector>

#include "DataEntry.hpp"

class AidLoader
{
public:
	AidLoader();
	AidLoader(std::string configFilename);
	const std::vector<DataEntry>& getDb() const;

private:
	std::vector<DataEntry> vDb;
};

#endif
