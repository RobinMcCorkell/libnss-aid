// PersistentLooper.hpp
// part of libnss-aid
//
// PersistentLooper header file
//
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

#ifndef HPP_PERSISTENTLOOPER
#define HPP_PERSISTENTLOOPER

#include <vector>

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

#endif
