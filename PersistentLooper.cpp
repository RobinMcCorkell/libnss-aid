// PersistentLooper.cpp
// part of libnss-aid
//
// PersistentLooper implementation
//
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

#include "PersistentLooper.hpp"

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
