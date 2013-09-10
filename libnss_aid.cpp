// libnss_aid.cpp
// part of libnss-aid
// 
// NSS module to return Android users and groups (Android IDs)
// 
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

// NSS headers
#include <nss.h>
#include <pwd.h>
#include <grp.h>

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
	__uid_t pw_uid; // User ID
	__git_t pw_gid; // Group ID
	char *pw_gecos; // Real name
	char *pw_dir; // Home directory
	char *pw_shell; // Shell program
}
 */

// ----
// Code
// ----

// State variables
AidLoader* loader;
PersistentLooper<DataEntry>* dataLooper;
 
// Allocate some space from the nss static buffer
void strcpyAllocate(const std::string from, char*& to, char*& buffer, size_t& buflen)
{
	size_t len = from.length() + 1;
	if ((buffer == nullptr) || (buflen < len)) {
		throw AllocateException("Insufficient space in buffer");
	}
	
	to = buffer;
	buffer += len;
	buflen -= len;
	
	from.copy(to, len - 1);
	to[len - 1] = '\0';
}

// Fill up a passwd struct with data from a DataEntry
void fillPasswd(const DataEntry& entry, struct passwd& result, char*& buffer, size_t& buflen)
{
	strcpyAllocate(entry.name.c_str(), result.pw_name, buffer, buflen);
	strcpyAllocate("x", result.pw_passwd, buffer, buflen);
	
	result.pw_uid = entry.id;
	result.pw_gid = entry.id;
	
	strcpyAllocate("", result.pw_gecos, buffer, buflen);
	strcpyAllocate("/", result.pw_dir, buffer, buflen);
	strcpyAllocate("/bin/false", result.pw_shell, buffer, buflen);
}

// Extern functions to be exposed to NSS
extern "C" enum nss_status _nss_aid_setpwent()
{
	loader = new AidLoader();
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
		dataLooper = new PersistentLooper<DataEntry>(loader->getDb());
	
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
	std::shared_ptr<AidLoader> myLoader(new AidLoader());
	
	auto entryIter = std::find(myLoader->getDb().begin(), myLoader->getDb().end(), std::string(name));
	
	if (entryIter != myLoader->getDb().end())
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
	std::shared_ptr<AidLoader> myLoader(new AidLoader());
	
	auto entryIter = std::find(myLoader->getDb().begin(), myLoader->getDb().end(), uid);
	
	if (entryIter != myLoader->getDb().end())
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
