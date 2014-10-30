/*
 */
#include <lex.hpp>
#include <parse_common.hpp>
#include <cassert>
#include <cstdio>

class LexEof: public std::exception {
public:
	LexEof() {}
	virtual const char* what() const throw() { return "EOF while lexing"; }
};

bool isident(char ch) {
	if( ch > 127 )	return true;
	if( ch == '_' )	return true;
	if( ch == '$' )	return true;
	if( 'a' <= ch && ch <= 'z') 	return true;
	if( 'A' <= ch && ch <= 'Z') 	return true;
	if( '0' <= ch && ch <= '9') 	return true;
	return false;
}

Lexer::Lexer(::std::istream& is):
	m_is(is),
	m_cached_valid(false)
{
}

Token Lexer::get_token()
{
	try
	{
		// Eat whitespace
		while( ::std::isspace(this->_getc()) )
			;
		this->_ungetc();
		
		switch( this->_getc() )
		{
		case '\n':
			return Token::single(TokNewline);
		case '#':
			return Token::single(TokHash);
		case '/':
			switch( this->_getc() )
			{
			case '/': {
				char ch;
				::std::string	val;
				while( (ch = this->_getc()) != '\n' )
					val.push_back(ch);
				this->_ungetc();
				return Token::string(TokComment, val);
				}
			case '*': {
				::std::string	val;
				for( ;; )
				{
					char ch;
					if( (ch = this->_getc()) != '*' ) {
						val.push_back(ch);
						continue ;
					}
					if( (ch = this->_getc()) != '/' ) {
						this->_ungetc();
						val.push_back('*');
						continue ;
					}
					val.push_back(ch);
					break;
				}
				return Token::string(TokComment, val);
				}
			case '=':
				return Token::single(TokSlashEqual);
			default:
				this->_ungetc();
				return Token::single(TokSlash);
			}
			break;
		case 'a'...'z':
		case 'A'...'Z':
		case '_':
		case '$': {
			::std::string	val;
			val.push_back(m_cached);
			while( isident(this->_getc()) )
				val.push_back(m_cached);
			this->_ungetc();
			return Token::string(TokIdent, val);
			}
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
