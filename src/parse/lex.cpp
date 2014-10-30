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

::std::ostream& operator<<(::std::ostream& os, enum eTokenType& tok)
{
	switch(tok)
	{
	case TokEOF:    	os << "TokEOF"; 	break;           
	case TokNewline:	os << "TokNewline";	break;
	case TokComment:	os << "TokComment";	break;
	case TokHash:   	os << "TokHash";	break;
	case TokIdent:  	os << "TokIdent";	break;
	case TokInteger:	os << "TokInteger";	break;
	case TokReal:   	os << "TokReal";	break;
	case TokBraceOpen:	os << "TokBraceOpen";	break;
	case TokBraceClose:	os << "TokBraceClose";	break;
	case TokParenOpen:	os << "TokParenOpen";	break;
	case TokParenClose:	os << "TokParenClose";	break;
	case TokSquareOpen:	os << "TokSquareOpen";	break;
	case TokSquareClose:	os << "TokSquareClose";	break;
	case TokLessThan:	os << "TokLessThan";	break;
	case TokGreaterThan:	os << "TokGreaterThan";	break;
	case TokPeriod: 	os << "TokPeriod";	break;
	case TokAmpersand:	os << "TokAmpersand";	break;
	case TokComma:  	os << "TokComma";	break;
	case TokSemicolon:	os << "TokSemicolon";	break;
	case TokStar:   	os << "TokStar";	break;
	case TokArrow:  	os << "TokArrow";	break;
	
	case TokPlus:   	os << "TokPlus";	break;
	case TokMinus:  	os << "TokMinus";	break;
	case TokSlash:  	os << "TokSlash";	break;
	case TokPercent:	os << "TokPercent";	break;
	case TokPipe:   	os << "TokPipe";	break;
	case TokCaret:  	os << "TokCaret";	break;
	case TokExclam: 	os << "TokExclam";	break;
	case TokDoubleAmpersand:os << "TokDoubleAmpersand";	break;
	case TokDoublePipe:	os << "TokDoublePipe";	break;
	case TokDoublePlus:	os << "TokDoublePlus";	break;
	case TokDoubleMinus:	os << "TokDoubleMinus";	break;
	case TokDoubleEqual:	os << "TokDoubleEqual";	break;
	case TokExclamEqual:	os << "TokExclamEqual";	break;

	case TokEqual:  	os << "TokEqual";	break;
	case TokPlusEqual:	os << "TokPlusEqual";	break;
	case TokMinusEqual:	os << "TokMinusEqual";	break;
	case TokStarEqual:	os << "TokStarEqual";	break;
	case TokSlashEqual:	os << "TokSlashEqual";	break;

	case TokRword_inline:	os << "TokRword_inline";	break;
	case TokRword_volatile:	os << "TokRword_volatile";	break;
	case TokRword_const:	os << "TokRword_const"; 	break;

	case TokRword_typedef:	os << "TokRword_typedef";	break;
	case TokRword_static:	os << "TokRword_static";	break;
	case TokRword_extern:	os << "TokRword_extern";	break;
	case TokRword_auto:	os << "TokRword_auto";  	break;
	case TokRword_register:	os << "TokRword_register";	break;

	case TokRword_struct:	os << "TokRword_struct";	break;
	case TokRword_enum:	os << "TokRword_enum";	break;
	case TokRword_union:	os << "TokRword_union";	break;

	case TokRword_void:	os << "TokRword_void";	break;
	case TokRword_char:	os << "TokRword_char";	break;
	case TokRword_short:	os << "TokRword_short";	break;
	case TokRword_int:	os << "TokRword_int";	break;
	case TokRword_long:	os << "TokRword_long";	break;
	case TokRword_signed:	os << "TokRword_signed";	break;
	case TokRword_unsigned:	os << "TokRword_unsigned";	break;
	case TokRword_float:	os << "TokRword_float"; 	break;
	case TokRword_double:	os << "TokRword_double";	break;
	case TokRword_Bool:	os << "TokRword_Bool";  	break;
	case TokRword_Complex:	os << "TokRword_Complex";	break;
        
	case TokRword_for:	os << "TokRword_for";	break;
	case TokRword_while:	os << "TokRword_while";	break;
	case TokRword_do:	os << "TokRword_do";	break;
	case TokRword_if:	os << "TokRword_if";	break;
	case TokRword_else:	os << "TokRword_else";	break;
	}
	return os;
}

::std::ostream& operator<<(::std::ostream& os, struct Token& tok)
{
	os << tok.type();
	return os;
}
