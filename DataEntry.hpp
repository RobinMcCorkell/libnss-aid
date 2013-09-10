// DataEntry.hpp
// part of libnss-aid
//
// DataEntry definition, a structure for IDs and names
//
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

#ifndef HPP_DATAENTRY
#define HPP_DATAENTRY

#include <string>

struct DataEntry
{
	std::string name;
	unsigned int id;
};

// Overloads
bool operator==(const DataEntry& a, const unsigned int& b);
bool operator==(const unsigned int& a, const DataEntry& b);
bool operator==(const DataEntry& a, const std::string& b);
bool operator==(const std::string& a, const DataEntry& b);

#endif
