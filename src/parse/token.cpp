/*
 */
#include <lex.hpp>

Token::Token():
	m_type(TokEOF)	// use EOF as the invalid value
{
}

Token::Token(enum eTokenType type):
	m_type(type)
{
	::std::cout << "new token (" << type << ")" << ::std::endl;
}

Token::~Token()
{
	
}

Token Token::eof()
{
	return Token(TokEOF);
}
Token Token::single(enum eTokenType type)
{
	Token	rv(type);
	// TODO: Assert it's a valid type
	return rv;
}
Token Token::string(enum eTokenType type, ::std::string val)
{
	Token	rv(type);
	rv.stringval = val; 
	// TODO: Assert it's a valid type
	return rv;
}

