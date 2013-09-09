// AllocateException.hpp
// part of libnss-aid
//
// AllocateException header file
//
// Copyright (C) 2013 Robin McCorkell <rmccorkell@karoshi.org.uk>

#ifndef HPP_ALLOCATEEXCEPTION
#define HPP_ALLOCATEEXCEPTION

#include <string>
#include <stdexcept>

class AllocateException : public std::runtime_error
{
public:
	AllocateException(const std::string& msg) : runtime_error(msg) {};
};

#endif
