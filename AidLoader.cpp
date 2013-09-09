// AidLoader.cpp
// part of libnss-aid
//
// AidLoader implementation
//
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

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
