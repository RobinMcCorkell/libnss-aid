// libnss_aid.cpp
// NSS module to return Android users and groups (Android IDs)

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

// NSS headers
#include <nss.h>
#include <pwd.h>
#include <grp.h>

// C headers
#include <cstdlib> // for realloc

// C++ headers
#include <string>
#include <memory>
#include <algorithm>

// My headers
#include "DataEntry.hpp"
#include "AidLoader.hpp"
#include "PersistentLooper.hpp"
#include "AllocateException.hpp"

/*
struct passwd
{
	char *pw_name; // Username
	char *pw_passwd; // Password
	uid_t pw_uid; // User ID
	gid_t pw_gid; // Group ID
	char *pw_gecos; // Real name
	char *pw_dir; // Home directory
	char *pw_shell; // Shell program
};

struct group
{
	char *gr_name; // Group name
	char *gr_passwd; // Password
	gid_t gr_gid; //Group ID
	char **gr_mem; // Member list
};

 */

// ----
// Code
// ----

// State variables
AidLoader* loader;
PersistentLooper<DataEntry>* dataLooper;

// Allocate some space from the nss static buffer
char* strcpyAllocate(const std::string& from, char*& to, char*& buffer, size_t& buflen)
{
	size_t len = from.length() + 1;
	if ((buffer == nullptr) || (buflen < len)) {
		throw AllocateException{"Insufficient space in buffer"};
	}

	to = buffer;
	buffer += len;
	buflen -= len;

	from.copy(to, len - 1);
	to[len - 1] = '\0';

	return to;
}

// Allocate some space from the nss static buffer for an array
char** arrayAllocate(size_t len, char**& to, char*& buffer, size_t& buflen)
{
	size_t pos = len;
	len += 1;
	len *= sizeof(char*);
	if ((buffer == nullptr) || (buflen < len)) {
		throw AllocateException{"Insufficient space in buffer"};
	}

	to = (char**) buffer;
	buffer += len;
	buflen -= len;

	to[pos] = nullptr;

	return to;
}

// Fill up a passwd struct with data from a DataEntry
void fillPasswd(const DataEntry& entry, struct passwd& result, char*& buffer, size_t& buflen)
{
	strcpyAllocate(entry.name, result.pw_name, buffer, buflen);
	strcpyAllocate("x", result.pw_passwd, buffer, buflen);

	result.pw_uid = entry.id;
	result.pw_gid = entry.id;

	strcpyAllocate("", result.pw_gecos, buffer, buflen);
	strcpyAllocate("/", result.pw_dir, buffer, buflen);
	strcpyAllocate("/bin/false", result.pw_shell, buffer, buflen);
}

// Fill up a group struct with data from a DataEntry
void fillGroup(const DataEntry& entry, struct group& result, char*& buffer, size_t& buflen)
{
	strcpyAllocate(entry.name, result.gr_name, buffer, buflen);
	strcpyAllocate("x", result.gr_passwd, buffer, buflen);

	result.gr_gid = entry.id;

	char** aMembers = arrayAllocate(entry.members.size(), result.gr_mem, buffer, buflen);
	size_t i = 0;
	for ( const auto& memberName : entry.members )
		strcpyAllocate(memberName, aMembers[i++], buffer, buflen);
}

// -------------
// NSS interface
// -------------

// NSS passwd functions
extern "C" enum nss_status _nss_aid_setpwent()
{
	loader = new AidLoader{};
	return NSS_STATUS_SUCCESS;
}

extern "C" enum nss_status _nss_aid_endpwent()
{
	if ( dataLooper != nullptr )
	{
		delete dataLooper;
		dataLooper = nullptr;
	}

	delete loader;
	loader = nullptr;
	return NSS_STATUS_SUCCESS;
}

extern "C" enum nss_status _nss_aid_getpwent_r(
	struct passwd *result,
	char *buffer,
	size_t buflen,
	int *errnop)
{
	if( loader == nullptr )
	{
		*errnop = EAGAIN;
		return NSS_STATUS_TRYAGAIN;
	}

	if ( dataLooper == nullptr )
		dataLooper = new PersistentLooper<DataEntry>{loader->getDb(), loader->getConfig()};

	DataEntry tmpEntry;
	typename PersistentLooper<DataEntry>::Status status = dataLooper->getNext(tmpEntry);

	switch (status)
	{
	case PersistentLooper<DataEntry>::Status::OK:
		break;
	case PersistentLooper<DataEntry>::Status::END:
		return NSS_STATUS_NOTFOUND;
		break;
	default:
		*errnop = EAGAIN;
		return NSS_STATUS_TRYAGAIN;
	}

	try
	{
		fillPasswd(tmpEntry, *result, buffer, buflen);
	}
	catch (AllocateException& err)
	{
		*errnop = ERANGE;
		return NSS_STATUS_TRYAGAIN;
	}

	return NSS_STATUS_SUCCESS;
}

extern "C" enum nss_status _nss_aid_getpwnam_r(
	const char *name,
	struct passwd *result,
	char *buffer,
	size_t buflen,
	int *errnop)
{
	AidLoader myLoader{};

	auto entryIter = std::find(myLoader.getDb().begin(), myLoader.getDb().end(), std::string{name});

	if (entryIter != myLoader.getDb().end())
	{
		try
		{
			fillPasswd(*entryIter, *result, buffer, buflen);
		}
		catch (AllocateException& err)
		{
			*errnop = ERANGE;
			return NSS_STATUS_TRYAGAIN;
		}
		return NSS_STATUS_SUCCESS;
	}

	return NSS_STATUS_NOTFOUND;
}

extern "C" enum nss_status _nss_aid_getpwuid_r(
	uid_t uid,
	struct passwd *result,
	char *buffer,
	size_t buflen,
	int *errnop)
{
	AidLoader myLoader{};

	auto entryIter = std::find(myLoader.getDb().begin(), myLoader.getDb().end(), uid);

	if (entryIter != myLoader.getDb().end())
	{
		try
		{
			fillPasswd(*entryIter, *result, buffer, buflen);
		}
		catch (AllocateException& err)
		{
			*errnop = ERANGE;
			return NSS_STATUS_TRYAGAIN;
		}
		return NSS_STATUS_SUCCESS;
	}

	return NSS_STATUS_NOTFOUND;
}

// NSS group functions
extern "C" enum nss_status _nss_aid_setgrent()
{
	loader = new AidLoader{};
	return NSS_STATUS_SUCCESS;
}

extern "C" enum nss_status _nss_aid_endgrent()
{
	if ( dataLooper != nullptr )
	{
		delete dataLooper;
		dataLooper = nullptr;
	}

	delete loader;
	loader = nullptr;
	return NSS_STATUS_SUCCESS;
}

extern "C" enum nss_status _nss_aid_getgrent_r(
	struct group *result,
	char *buffer,
	size_t buflen,
	int *errnop)
{
	if( loader == nullptr )
	{
		*errnop = EAGAIN;
		return NSS_STATUS_TRYAGAIN;
	}

	if ( dataLooper == nullptr )
		dataLooper = new PersistentLooper<DataEntry>{loader->getDb(), loader->getConfig()};

	DataEntry tmpEntry;
	typename PersistentLooper<DataEntry>::Status status = dataLooper->getNext(tmpEntry);

	switch (status)
	{
	case PersistentLooper<DataEntry>::Status::OK:
		break;
	case PersistentLooper<DataEntry>::Status::END:
		return NSS_STATUS_NOTFOUND;
		break;
	default:
		*errnop = EAGAIN;
		return NSS_STATUS_TRYAGAIN;
	}

	try
	{
		fillGroup(tmpEntry, *result, buffer, buflen);
	}
	catch (AllocateException& err)
	{
		*errnop = ERANGE;
		return NSS_STATUS_TRYAGAIN;
	}

	return NSS_STATUS_SUCCESS;
}

extern "C" enum nss_status _nss_aid_getgrnam_r(
	const char *name,
	struct group *result,
	char *buffer,
	size_t buflen,
	int *errnop)
{
	AidLoader myLoader{};

	auto entryIter = std::find(myLoader.getDb().begin(), myLoader.getDb().end(), std::string{name});

	if (entryIter != myLoader.getDb().end())
	{
		try
		{
			fillGroup(*entryIter, *result, buffer, buflen);
		}
		catch (AllocateException& err)
		{
			*errnop = ERANGE;
			return NSS_STATUS_TRYAGAIN;
		}
		return NSS_STATUS_SUCCESS;
	}

	return NSS_STATUS_NOTFOUND;
}

extern "C" enum nss_status _nss_aid_getgrgid_r(
	gid_t gid,
	struct group *result,
	char *buffer,
	size_t buflen,
	int *errnop)
{
	AidLoader myLoader{};

	auto entryIter = std::find(myLoader.getDb().begin(), myLoader.getDb().end(), gid);

	if (entryIter != myLoader.getDb().end())
	{
		try
		{
			fillGroup(*entryIter, *result, buffer, buflen);
		}
		catch (AllocateException& err)
		{
			*errnop = ERANGE;
			return NSS_STATUS_TRYAGAIN;
		}
		return NSS_STATUS_SUCCESS;
	}

	return NSS_STATUS_NOTFOUND;
}

// Returns a list of groups for a user
// Documentation is scarce (read: non-existent) so the paramteters are assumed
//  char *user - user to search for
//  gid_t skipgroup - group to not include in results
//  size_t *start - where to write in the array, incremented ???
//  size_t *buflen - length of buffer in gid_t entries
//  gid_t **buffer - pointer to array of returned GIDs
//  size_t limit - maximum size of array
//  int *errnop - error status
extern "C" enum nss_status _nss_aid_initgroups_dyn(
	const char *user,
	gid_t skipgroup,
	size_t *start,
	size_t *buflen,
	gid_t **buffer,
	size_t limit,
	int *errnop)
{
	AidLoader myLoader{};

	auto entryIter = std::find(myLoader.getDb().begin(), myLoader.getDb().end(), std::string{user});

	if (entryIter != myLoader.getDb().end())
	{
		try
		{
			for (const auto& entry : myLoader.getDb())
			{
				// skipgroup check
				if ( entry.id == skipgroup )
					continue;

				if ( std::find(entry.members.begin(), entry.members.end(), std::string{user}) != entry.members.end() )
				{
					// Limit check
					if ( (limit > 0) && (*start >= limit) )
						break;

					// Buffer size check
					if ( (*start) >= (*buflen) )
					{
						size_t new_buflen = 2 * (*buflen);
						if ( (limit > 0) && (new_buflen > limit) )
							new_buflen = limit;
						gid_t *new_buffer = (gid_t*) realloc(*buffer, new_buflen * sizeof(gid_t));
						if (new_buffer == nullptr)
							throw AllocateException{"Unable to reallocate memory"};
						*buffer = new_buffer;
						*buflen = new_buflen;
					}

					// Add GID to list
					(*buffer)[(*start)++] = entry.id;
				}
			}
		}
		catch (AllocateException& err)
		{
			*errnop = ERANGE;
			return NSS_STATUS_TRYAGAIN;
		}
		return NSS_STATUS_SUCCESS;
	}

	return NSS_STATUS_NOTFOUND;
}
