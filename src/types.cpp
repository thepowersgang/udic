/*
 */
#include <types.hpp>

// === CODE ===
TypeRef TypeImpl::integer(IntClass::Size size, bool is_signed)
{
	return TypeRef(size, is_signed);
}

TypeRef TypeImpl::real(RealClass::Size size)
{
	return TypeRef(size, false);
}
TypeRef TypeImpl::complex_(RealClass::Size size)
{
	return TypeRef(size, true);
}

