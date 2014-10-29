/*
 */
#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <memory>

struct IntClass
{
	enum {
		INT_CHAR,
		INT_SHORT,
		INT_INT,
		INT_LONG,
		INT_LONGLONG,
	} size;
	bool	issigned;
};

class TypeImpl;

typedef ::std::shared_ptr<TypeImpl>	TypeRef;

class TypeImpl
{

};

#endif
