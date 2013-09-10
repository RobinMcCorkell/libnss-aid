// DataEntry.cpp
// part of libnss-aid
//
// DataEntry operator overloads
//
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

#include "DataEntry.hpp"

bool operator==(const DataEntry& a, const unsigned int& b)
{
	return a.id == b;
}
bool operator==(const unsigned int& a, const DataEntry& b)
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

