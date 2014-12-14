/*
 */
#include <lex.hpp>
#include <parse_common.hpp>
#include <cassert>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <cstdlib>  // strtol
#include <typeinfo>

bool isident(char ch) {
	//::std::cout << "isident(ch = " << (int)ch << ")\n";
	if( ch > 127 )	return true;
	if( ch == '_' )	return true;
	if( ch == '$' )	return true;
	if( 'a' <= ch && ch <= 'z') 	return true;
	if( 'A' <= ch && ch <= 'Z') 	return true;
	if( '0' <= ch && ch <= '9') 	return true;
	return false;
}

Lexer::Lexer(::std::istream& is):
    m_istream(is),
    m_last_char_valid(false)
{
}

#define LINECOMMENT -1
#define BLOCKCOMMENT -2
#define SINGLEQUOTE -3
#define DOUBLEQUOTE -4

// NOTE: This array must be kept reverse sorted
#define TOKENT(str, sym)    {sizeof(str)-1, str, sym}
static const struct {
    unsigned char len;
    const char* chars;
    signed int type;
} TOKENMAP[] = {
  TOKENT("!" , TokExclam),
  TOKENT("!=", TokExclamEqual),
  TOKENT("\"", DOUBLEQUOTE),
  TOKENT("#",  TokHash),
  TOKENT("##", TokDoubleHash),
  TOKENT("%" , TokPercent),
  TOKENT("%=", TokPercentEqual),
  TOKENT("&" , TokAmpersand),
  TOKENT("&&", TokDoubleAmpersand),
  TOKENT("&=", TokAmpersandEqual),
  TOKENT("'" , SINGLEQUOTE),
  TOKENT("(" , TokParenOpen),
  TOKENT(")" , TokParenClose),
  TOKENT("*" , TokStar),
  TOKENT("*=", TokStarEqual),
  TOKENT("+" , TokPlus),
  TOKENT("+=", TokPlusEqual),
  TOKENT("," , TokComma),
  TOKENT("-" , TokMinus),
  TOKENT("-=", TokMinusEqual),
  TOKENT("->", TokArrow),
  TOKENT(".",  TokPeriod),
  TOKENT("..", TokDoublePeriod),
  TOKENT("...",TokTriplePeriod),
  TOKENT("/" , TokSlash),
  TOKENT("/*", BLOCKCOMMENT),
  TOKENT("//", LINECOMMENT),
  TOKENT("/=", TokSlashEqual),
  // 0-9 :: Handled by fallback code
  TOKENT(":",  TokColon),
  TOKENT(";",  TokSemicolon),
  TOKENT("<",  TokLessThan),
  TOKENT("<<", TokDoubleLessThan),
  TOKENT("<=", TokLessThanEqual),
  TOKENT("=" , TokEqual),
  TOKENT("==", TokDoubleEqual),
  TOKENT(">",  TokGreaterThan),
  TOKENT(">>", TokDoubleGreaterThan),
  TOKENT(">=", TokGreaterThanEqual),
  TOKENT("?",  TokQuestionMark),
  TOKENT("@",  TokAt),
  // A-Z :: Elsewhere
  TOKENT("[",  TokSquareOpen),
  TOKENT("\\", TokBackslash),
  TOKENT("]",  TokSquareClose),
  TOKENT("^",  TokCaret),
  TOKENT("`",  TokBacktick),

  TOKENT("{",  TokBraceOpen),
  TOKENT("|",  TokPipe),
  TOKENT("|=", TokPipeEqual),
  TOKENT("||", TokDoublePipe),
  TOKENT("}",  TokBraceClose),
  TOKENT("~",  TokTilde),
};
#define LEN(arr)    (sizeof(arr)/sizeof(arr[0]))
static const struct {
    unsigned char len;
    const char* chars;
    signed int type;
} RWORDS[] = {
  TOKENT("break",   TokRword_break),
  TOKENT("const",   TokRword_const),
  TOKENT("continue",TokRword_continue),
  TOKENT("do",      TokRword_do),
  TOKENT("else",    TokRword_else),
  TOKENT("enum",    TokRword_enum),
  TOKENT("extern",  TokRword_extern),
  TOKENT("for",     TokRword_for),
  TOKENT("if",      TokRword_if),
  TOKENT("return",  TokRword_return),
  //TOKENT("sizeof",  TokRword_sizeof),
  TOKENT("static",  TokRword_static),
  TOKENT("struct",  TokRword_struct),
  TOKENT("typedef", TokRword_typedef),
  //TOKENT("typeof",  TokRword_typeof),
  TOKENT("while",   TokRword_while),
};

signed int Lexer::getSymbol()
{
    char ch = this->getc();
    // 1. lsearch for character
    // 2. Consume as many characters as currently match
    // 3. IF: a smaller character or, EOS is hit - Return current best
    unsigned ofs = 0;
    signed int best = 0;
    for(unsigned i = 0; i < LEN(TOKENMAP); i ++)
    {
        const char* const chars = TOKENMAP[i].chars;
        const size_t len = TOKENMAP[i].len;

        //::std::cout << "ofs=" << ofs << ", chars[ofs] = " << chars[ofs] << ", ch = " << ch << ", len = " << len << ::std::endl;

        if( ofs >= len || chars[ofs] > ch ) {
            this->ungetc();
            return best;
        }

        while( chars[ofs] && chars[ofs] == ch )
        {
            ch = this->getc();
            ofs ++;
        }
        if( chars[ofs] == 0 )
        {
            best = TOKENMAP[i].type;
        }
    }

    this->ungetc();
    return best;
}

Token Lexer::get_token()
{
    try
    {
        char ch = this->getc();

        // Newline - special handling
        if( ch == '\n' ) {
            return Token(TokNewline);
        }

        // Whitespace, collapse into one
        if( ::std::isspace(ch) )
        {
            while( ::std::isspace(this->getc()) )
                ;
            this->ungetc();
            return Token(TokWhitespace);
        }
        this->ungetc();

        // Check for a basic token
        const signed int sym = this->getSymbol();
        // - If none matched, check for a number, resserved word, or an identifier
        if( sym == 0 )
        {
            char ch = this->getc();
            if( ::std::isdigit(ch) )
            {
                if( ch == '0' )
                {
                    // Octal/hex handling
                    ch = this->getc();
                    if( ch == 'x' ) {
                        return this->parse_number(16);
                    }
                    else {
                        this->ungetc();
                        return this->parse_number(8);
                    }
                }
                else {
                    this->ungetc();
                    return this->parse_number(10);
                }
            }
            else if( isident(ch) )
            {
                ::std::string   str;
                while( isident(ch) )
                {
                    str.push_back(ch);
                    ch = this->getc();
                }
                this->ungetc();

                for( unsigned int i = 0; i < LEN(RWORDS); i ++ )
                {
                    if( str < RWORDS[i].chars ) break;
                    if( str == RWORDS[i].chars )    return Token((enum eTokenType)RWORDS[i].type);
                }
                return Token(TokIdent, str);
            }
            else
            {
                throw ParseError::UnknownChar(ch);
            }
        }
        else if( sym > 0 )
        {
            return Token((enum eTokenType)sym);
        }
        else
        {
            switch(sym)
            {
            case LINECOMMENT: {
                // Line comment
                ::std::string   str;
                char ch = this->getc();
                while(ch != '\n' && ch != '\r')
                {
                    str.push_back(ch);
                    ch = this->getc();
                }
                return Token(TokComment, str); }
            case BLOCKCOMMENT: {
                ::std::string   str;
                while(true)
                {
                    if( ch == '*' ) {
                        ch = this->getc();
                        if( ch == '/' ) break;
                        this->ungetc();
                    }
                    str.push_back(ch);
                    ch = this->getc();
                }
                return Token(TokComment, str); }
            case SINGLEQUOTE: {
                char firstchar = this->getc();
                if( firstchar != '\\' ) {
                    ch = this->getc();
                    return Token::integer(ch, IntClass::INT_INT, false);
                }
                else {
                    // Character constant with an escape code
                    uint32_t val = this->parseEscape('\'');
                    if(this->getc() != '\'') {
                        throw ParseError::Todo("Proper error for lex failures");
                    }
                    return Token::integer(val, IntClass::INT_INT, false);
                }
                break; }
            case DOUBLEQUOTE:
                throw ParseError::Todo("Strings");
                break;
            default:
                assert(!"bugcheck");
            }
        }
    }
    catch(const Lexer::EndOfFile& e)
    {
        return Token(TokEOF);
    }
    //assert(!"bugcheck");
}

uint32_t Lexer::parseEscape(char enclosing)
{
    char ch = this->getc();
    switch(ch)
    {
    case 'u': {
        // Unicode (up to six hex digits)
        uint32_t    val = 0;
        ch = this->getc();
        if( !isxdigit(ch) )
            throw ParseError::Todo("Proper lex error for escape sequences");
        while( isxdigit(ch) )
        {
            char    tmp[2] = {ch, 0};
            val *= 16;
            val += ::std::strtol(tmp, NULL, 16);
            ch = this->getc();
        }
        this->ungetc();
        return val; }
    case '\\':
        return '\\';
    default:
        if( ch == enclosing ) {
            return enclosing;
        }
        else {
            throw ParseError::Todo("Proper lex error for escape sequences");
        }
    }
}

::std::pair<::std::string,bool>	Lexer::read_cpp_string()
{
	::std::string	rv;
	char	ch = ' ';
	switch(this->getc())
	{
	case '<':
		while( (ch = this->getc()) != '>' )
			rv.push_back(ch);
		return ::std::make_pair(rv, true);
	case '"':
		while( (ch = this->getc()) != '"' )
			rv.push_back(ch);
		return ::std::make_pair(rv, false);
	default:
		throw ParseError::SyntaxError("Expected preprocessor string (<str>, or \"str\")");
	}
}

char Lexer::getc()
{
    if( m_last_char_valid )
    {
        m_last_char_valid = false;
    }
    else
    {
		m_last_char = m_istream.get();
		if( m_istream.eof() )
			throw Lexer::EndOfFile();
    }
    //::std::cout << "getc(): '" << m_last_char << "'" << ::std::endl;
    return m_last_char;
}

void Lexer::ungetc()
{
//    ::std::cout << "putback(): " << m_last_char_valid << " '" << m_last_char << "'" << ::std::endl;
    assert(!m_last_char_valid);
    m_last_char_valid = true;
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
	while( getdigit(base, this->getc(), digit) )
	{
		rv *= base;
		rv += digit;
	}
	this->ungetc();
	return rv;
}

Token Lexer::parse_number(unsigned int base)
{
	// 1. Get number
	unsigned long long val = this->read_number(base);

	char ch = this->getc();
	if( ch == '.' )
	{
		throw ParseError::Todo("Lexer::parse_number - decimals");
	}
	else
	{
		bool is_unsigned = false;
		bool is_long = false;
		bool is_longlong = false;
		if( ch == 'u' || ch == 'U' ) { is_unsigned = true; ch = this->getc(); }
		if( ch == 'l' || ch == 'L' ) { is_long     = true; ch = this->getc(); }
		if( ch == 'l' || ch == 'L' ) { is_longlong = true; ch = this->getc(); }
		this->ungetc();
		auto size = (!is_long ? IntClass::INT_INT : (!is_longlong ? IntClass::INT_LONG : IntClass::INT_LONGLONG));

		return Token::integer(val, size, !is_unsigned);
	}

	throw ParseError::Todo("Lexer::parse_number");
}
