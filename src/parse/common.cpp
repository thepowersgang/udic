/*
 */
#include <parse_common.hpp>
#include <sstream>

ParseError::Todo::Todo(const char *reason):
	m_reasonstr(reason)
{
}
const char *ParseError::Todo::what() const throw()
{
	return "TODO";
}
::std::string ParseError::Todo::message() const
{
	return ::std::string(m_reasonstr);
}

ParseError::UnknownChar::UnknownChar(char ch):
	m_ch(ch)
{
}
const char *ParseError::UnknownChar::what() const throw()
{
	return "Unknown character";
}
::std::string ParseError::UnknownChar::message() const
{
	::std::stringstream	ss;
	ss << "'" << m_ch << "' (" << (int)m_ch << ")";
	return ss.str();
}


