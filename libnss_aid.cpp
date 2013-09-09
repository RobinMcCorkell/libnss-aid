// libnss_aid.cpp
// 
// NSS module to return Android users and groups (Android IDs)
// 
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

// NSS headers
#include <nss.h>
#include <pwd.h>
#include <grp.h>

// C compatibility headers
#include <cstring>

// C++ headers
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

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

// ----------
// Data entry
// ----------

struct DataEntry
{
	std::string name;
	unsigned int id;
};

// ------------
// Loader class
// ------------
class AidLoader
{
public:
	AidLoader(std::string configFilename = "/etc/libnss-aid.conf");
	const std::vector<DataEntry>& getDb() const;
	
private:
	std::vector<DataEntry> vDb;
};

// AidLoader functions
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

// ------------
// Looper class
// ------------
template <class vType>
class PersistentLooper
{
public:
	enum class Status { OK, END, ERROR };

	PersistentLooper( const std::vector<vType>& loopVector );
	Status getNext(vType& result);
	void resetLoop();
	
private:
	const std::vector<vType>& vLoop;
	typename std::vector<vType>::const_iterator iLoop;
};

// PersistentLooper functions
template <class vType>
PersistentLooper<vType>::PersistentLooper( const std::vector<vType>& loopVector ) : vLoop(loopVector)
{
	iLoop = vLoop.begin();
}

template <class vType>
typename PersistentLooper<vType>::Status PersistentLooper<vType>::getNext(vType& result)
{
	if ( iLoop == vLoop.end() )
		return Status::END;
	
	result = *iLoop;
	++iLoop;
	return Status::OK;
}

template <class vType>
void PersistentLooper<vType>::resetLoop()
{
	iLoop = vLoop.begin();
}

// ----------------
// Custom Exception
// ----------------

class AllocateException : public std::runtime_error
{
public:
	AllocateException(const std::string& msg) : runtime_error(msg) {};
};

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
		strcpyAllocate(tmpEntry.name.c_str(), result->pw_name, buffer, buflen);
		strcpyAllocate("x", result->pw_passwd, buffer, buflen);
		
		result->pw_uid = tmpEntry.id;
		result->pw_gid = tmpEntry.id;
		
		strcpyAllocate("", result->pw_gecos, buffer, buflen);
		strcpyAllocate("/", result->pw_dir, buffer, buflen);
		strcpyAllocate("/bin/false", result->pw_shell, buffer, buflen);
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
	
	if ( dataLooper == nullptr )
		dataLooper = new PersistentLooper<DataEntry>(myLoader->getDb());
	
	DataEntry tmpEntry;
	typename PersistentLooper<DataEntry>::Status status = PersistentLooper<DataEntry>::Status::OK;
	bool found = false;
	
	while ( status == PersistentLooper<DataEntry>::Status::OK )	
	{
		status = dataLooper->getNext(tmpEntry);
		
		if ( status == PersistentLooper<DataEntry>::Status::ERROR )
		{
			*errnop = EAGAIN;
			return NSS_STATUS_TRYAGAIN;
		}
		
		if ( tmpEntry.name == name )
		{
			found = true;
			break;
		}
	}
	
	if (found)
	{
		try
		{
			strcpyAllocate(tmpEntry.name.c_str(), result->pw_name, buffer, buflen);
			strcpyAllocate("x", result->pw_passwd, buffer, buflen);
			
			result->pw_uid = tmpEntry.id;
			result->pw_gid = tmpEntry.id;
			
			strcpyAllocate("", result->pw_gecos, buffer, buflen);
			strcpyAllocate("/", result->pw_dir, buffer, buflen);
			strcpyAllocate("/bin/false", result->pw_shell, buffer, buflen);
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
