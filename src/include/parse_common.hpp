/*
 */
#ifndef _PARSE_COMMON_HPP_
#define _PARSE_COMMON_HPP_

#include <stdexcept>

namespace ParseError {

class Base:
	public ::std::exception
{
public:
	virtual ::std::string	message() const = 0;
};

class Todo:
	public Base
{
	const char* m_reasonstr;
public:
	Todo(const char* reason);
	virtual const char *what() const throw();
	virtual ::std::string	message() const;
};

class UnknownChar:
	public Base
{
	char	m_ch;
public:
	UnknownChar(char ch);
	virtual const char *what() const throw();
	virtual ::std::string	message() const;
};


}	// namespace ParseError

#endif

