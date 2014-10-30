/*
 */
#include <types.hpp>

// === CODE ===
TypeRef TypeImpl::integer(IntClass::Size size, bool is_signed)
{
	return ::std::shared_ptr<TypeImpl>(new TypeImpl(size, is_signed));
}

TypeRef TypeImpl::real(RealClass::Size size)
{
	return ::std::shared_ptr<TypeImpl>(new TypeImpl(size, false));
}
TypeRef TypeImpl::complex_(RealClass::Size size)
{
	return ::std::shared_ptr<TypeImpl>(new TypeImpl(size, true));
}

TypeImpl::TypeImpl(IntClass::Size size, bool is_signed)
{
}

TypeImpl::TypeImpl(RealClass::Size size, bool is_complex)
{
}

