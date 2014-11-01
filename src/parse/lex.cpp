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
	::std::cout << "isident(ch = " << (int)ch << ")\n";
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
		switch( this->_getc() )
		{
		case ' ':
		case '\t':
			while( ::std::isspace(this->_getc()) )
				;
			this->_ungetc();
			return Token::single(TokWhitespace);
		case '\n':
			return Token::single(TokNewline);
		case '#':
			switch( this->_getc() )
			{
			case '#':
				return Token::single(TokDoubleHash);
			default:
				this->_ungetc();
				return Token::single(TokHash);
			}
			break;
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
		case '0':
			switch( this->_getc() )
			{
			case 'x':
				return this->parse_number(16);
			case '0' ... '9':
				this->_getc();
				return this->parse_number(8);
			default:
				this->_getc();
				return this->parse_number(10);
			}
			break;
		case '1' ... '9':
			return this->parse_number(10);
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
		m_cached = m_is.get();
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

bool getdigit(unsigned int base, unsigned char ch, unsigned int& digit)
{
	if( base <= 10 )
	{
		if( '0' <= ch && ch <= '0'+base-1 ) {
			digit = ch - '0';
			return true;
		}
		return false;
	}
	else
	{
		if( '0' <= ch && ch <= '9' ) {
			digit = ch - '0';
			return true;
		}
		char last_letter = 'A' + base-10 - 1;
		if( 'A' <= ch && ch <= last_letter ) {
			digit = ch - 'A' + 10;
			return true;
		}
		if( 'a' <= ch && ch <= last_letter + 'a'-'A' ) {
			digit = ch - 'a' + 10;
			return true;
		}
		return false;
	}
}

unsigned long long Lexer::read_number(unsigned int base)
{
	unsigned int digit;
	unsigned long long rv = 0;
	while( getdigit(base, this->_getc(), digit) )
	{
		rv *= base;
		rv += digit;
	}
	this->_ungetc();
	return rv;
}

Token Lexer::parse_number(unsigned int base)
{
	// 1. Get number
	unsigned long long val = this->read_number(base);
	
	char ch = this->_getc();
	if( ch == '.' )
	{
		throw ParseError::Todo("Lexer::parse_number - decimals");
	}
	else
	{
		bool is_unsigned = false;
		bool is_long = false;
		bool is_longlong = false;
		if( ch == 'u' || ch == 'U' ) { is_unsigned = true; ch = this->_getc(); }
		if( ch == 'l' || ch == 'L' ) { is_long     = true; ch = this->_getc(); }
		if( ch == 'l' || ch == 'L' ) { is_longlong = true; ch = this->_getc(); }
		auto size = (!is_long ? IntClass::INT_INT : (!is_longlong ? IntClass::INT_LONG : IntClass::INT_LONGLONG));
	
		return Token::integer(val, size, !is_unsigned);
	}
	
	throw ParseError::Todo("Lexer::parse_number");
}

