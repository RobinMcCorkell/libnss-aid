// PersistentLooper.cpp
// PersistentLooper implementation

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

#include "PersistentLooper.hpp"

template <class vType>
PersistentLooper<vType>::PersistentLooper( const std::vector<vType>& loopVector, const boost::program_options::variables_map& config ) : vLoop(loopVector), mConfig(config)
{
	iLoop = vLoop.begin();
}

template <class vType>
typename PersistentLooper<vType>::Status PersistentLooper<vType>::getNext(vType& result)
{
	if ( mConfig["enumerate"].as<bool>() == false )
		return Status::END;
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
