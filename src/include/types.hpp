/*
 */
#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <memory>

struct IntClass
{
	enum Size {
		INT_CHAR,
		INT_SHORT,
		INT_INT,
		INT_LONG,
		INT_LONGLONG,
	} size;
	bool	issigned;
};

struct RealClass
{
	enum Size {
		REAL_FLOAT,
		REAL_DOUBLE,
		REAL_LONGDOUBLE,
	} size;
	bool is_complex;
};

class TypeImpl;

typedef ::std::shared_ptr<TypeImpl>	TypeRef;

class TypeImpl
{
public:
	static TypeRef	integer(IntClass::Size size, bool is_signed);
	static TypeRef	complex_(RealClass::Size size);
	static TypeRef	real(RealClass::Size size);
private:
	TypeImpl(IntClass::Size size, bool is_signed);
	TypeImpl(RealClass::Size size, bool is_complex);
};

#endif
