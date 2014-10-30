/*
 */
#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <sstream>

template <typename Arg>
void FORMAT_int(::std::stringstream& ss, Arg arg1)
{
	ss << arg1;
}
template <typename Arg1, typename ...Args>
void FORMAT_int(::std::stringstream& ss, Arg1 arg1, Args... args)
{
	ss << arg1;
	FORMAT_int(ss, args...);
}

template <typename ...Args>
::std::string FORMAT(Args... args)
{
	::std::stringstream	ss;
	FORMAT_int(ss, args...);
	return ss.str();
}

#endif

