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

#include <fstream>

AidLoader::AidLoader() : AidLoader("/etc/libnss-aid.conf")
{
}

AidLoader::AidLoader(const std::string& configFilename)
{
	//Process configuration file
	std::ifstream configFile(configFilename);

	boost::program_options::options_description config;
	config.add_options()
		("enumerate", boost::program_options::value<bool>()->default_value(false))
		("dynamic", boost::program_options::value<bool>()->default_value(true))
		("dynamic-end", boost::program_options::value<uid_t>()->default_value(100000));

	boost::program_options::store(
		boost::program_options::parse_config_file(configFile, config),
		mConfig);

	//Generate vDb entries
	{
		vDb.push_back( { "aid_system", 1000, {"aid_system"} } );
		vDb.push_back( { "aid_radio", 1001, {"aid_radio"} } );
		vDb.push_back( { "aid_bluetooth", 1002, {"aid_bluetooth"} } );
		vDb.push_back( { "aid_graphics", 1003, {"aid_graphics"} } );
		vDb.push_back( { "aid_input", 1004, {"aid_input"} } );
		vDb.push_back( { "aid_audio", 1005, {"aid_audio"} } );
		vDb.push_back( { "aid_camera", 1006, {"aid_camera"} } );
		vDb.push_back( { "aid_log", 1007, {"aid_log"} } );
		vDb.push_back( { "aid_compass", 1008, {"aid_compass"} } );
		vDb.push_back( { "aid_mount", 1009, {"aid_mount"} } );
		vDb.push_back( { "aid_wifi", 1010, {"aid_wifi"} } );
		vDb.push_back( { "aid_adb", 1011, {"aid_adb"} } );
		vDb.push_back( { "aid_install", 1012, {"aid_install"} } );
		vDb.push_back( { "aid_media", 1013, {"aid_media"} } );
		vDb.push_back( { "aid_dhcp", 1014, {"aid_dhcp"} } );
		vDb.push_back( { "aid_sdcard_rw", 1015, {"aid_sdcard_rw"} } );
		vDb.push_back( { "aid_vpn", 1016, {"aid_vpn"} } );
		vDb.push_back( { "aid_keystore", 1017, {"aid_keystore"} } );

		vDb.push_back( { "aid_shell", 2000, {"aid_shell"} } );
		vDb.push_back( { "aid_cache", 2001, {"aid_cache"} } );
		vDb.push_back( { "aid_diag", 2002, {"aid_diag"} } );

		vDb.push_back( { "aid_net_bt_admin", 3001, {"aid_net_bt_admin"} } );
		vDb.push_back( { "aid_net_bt", 3002, {"aid_net_bt"} } );
		vDb.push_back( { "aid_inet", 3003, {"aid_inet"} } );
		vDb.push_back( { "aid_net_raw", 3004, {"aid_net_raw"} } );
		vDb.push_back( { "aid_net_admin", 3005, {"aid_net_admin"} } );

		vDb.push_back( { "aid_misc", 9998, {"aid_misc"} } );
		vDb.push_back( { "aid_nobody", 9999, {"aid_nobody"} } );
	}

	//Parse group file and append group entries as necessary
}

const std::vector<DataEntry>& AidLoader::getDb() const
{
	return vDb;
}

const boost::program_options::variables_map& AidLoader::getConfig() const
{
	return mConfig;
}
