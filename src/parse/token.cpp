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

