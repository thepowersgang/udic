/*
 */
#include <lex.hpp>

class LexEof: public std::exception {
public:
	LexEof() {}
	virtual const char* what() const throw() { return "EOF while lexing"; }
};

Lexer::Lexer(::std::istream& is):
	m_is(is)
{
}

Token Lexer::get_token()
{
	try
	{
		while( ::std::isspace(this->_getc()) )
		{
		}
		this->_ungetc();
		
		switch( this->_getc() )
		{
		case '#':	return Token::single(TokHash);
		case '/':
			switch( this->_getc() )
			{
			case '/':
				throw ParseError::Misc("C99/C++ comments are not allowed in UDI code");
				break;
			case '*':
				return ParseError::Todo("C comments");
			case '=':
				return Token::single(TokSlashEqual);
			default:
				this->_ungetc();
				return Token::single(TokSlash);
			}
			break;
		default:
			throw ParseError::UnknownChar(this->m_cached);
		}
	}
	catch( const LexEof& e )
	{
		return Token::eof();
	}
}

char Lexer::_getc()
{
	if( !m_cached_valid )
	{
		m_is >> m_cached;
		if( m_is.eof() )
			throw LexEof();
	}
	m_cached_valid = false;
	return m_cached;
}

void Lexer::_ungetc()
{
	assert( !m_cached_valid );
	m_cached_valid = true;
}
