/*
 */
#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <memory>
#include <vector>

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

class TypeRef;

struct Composite
{
    typedef ::std::pair< ::std::string, TypeRef >   item_t;

    bool    m_is_union;
    ::std::vector<item_t>   m_items;

    struct TagStruct {};
    Composite(TagStruct, ::std::vector<item_t> items):
        m_is_union(false),
        m_items(items)
    {
    }

    struct TagUnion {};
    Composite(TagUnion, ::std::vector<item_t> items):
        m_is_union(true),
        m_items(items)
    {
    }

};

class TypeRef
{
    enum {
        CLASS_UNBOUND,
        CLASS_COMPOSITE,
        CLASS_INT,
        CLASS_FLOAT,
    }   m_class;

    union {
        struct {
            IntClass::Size  size;
            bool    is_signed;
        } integer;
        struct {
            RealClass::Size size;
            bool    is_complex;
        } floating;
        const Composite* composite;
    }   m_data;

public:
    TypeRef():
        m_class(CLASS_UNBOUND)
    {
    }
    TypeRef(const Composite& composite):
        m_class(CLASS_COMPOSITE)
    {
        m_data.composite = &composite;
    }
    TypeRef(IntClass::Size size, bool is_signed):
        m_class(CLASS_INT)
    {
        m_data.integer.size = size;
        m_data.integer.is_signed = is_signed;
    }
    TypeRef(RealClass::Size size, bool is_complex):
        m_class(CLASS_FLOAT)
    {
        m_data.floating.size = size;
        m_data.floating.is_complex = is_complex;
    }

    bool valid() const { return m_class != CLASS_UNBOUND; }
};

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
