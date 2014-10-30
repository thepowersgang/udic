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

const char *Token::enumname(enum eTokenType type)
{
	switch(type)
	{
	case TokEOF:    	return "TokEOF";            
	case TokNewline:	return "TokNewline";
	case TokComment:	return "TokComment";
	case TokHash:   	return "TokHash";
	case TokIdent:  	return "TokIdent";
	case TokInteger:	return "TokInteger";
	case TokReal:   	return "TokReal";
	case TokBraceOpen:	return "TokBraceOpen";
	case TokBraceClose:	return "TokBraceClose";
	case TokParenOpen:	return "TokParenOpen";
	case TokParenClose:	return "TokParenClose";
	case TokSquareOpen:	return "TokSquareOpen";
	case TokSquareClose:	return "TokSquareClose";
	case TokLessThan:	return "TokLessThan";
	case TokGreaterThan:	return "TokGreaterThan";
	case TokPeriod: 	return "TokPeriod";
	case TokAmpersand:	return "TokAmpersand";
	case TokComma:  	return "TokComma";
	case TokSemicolon:	return "TokSemicolon";
	case TokStar:   	return "TokStar";
	case TokArrow:  	return "TokArrow";
	
	case TokPlus:   	return "TokPlus";
	case TokMinus:  	return "TokMinus";
	case TokSlash:  	return "TokSlash";
	case TokPercent:	return "TokPercent";
	case TokPipe:   	return "TokPipe";
	case TokCaret:  	return "TokCaret";
	case TokExclam: 	return "TokExclam";
	case TokDoubleAmpersand:return "TokDoubleAmpersand";
	case TokDoublePipe:	return "TokDoublePipe";
	case TokDoublePlus:	return "TokDoublePlus";
	case TokDoubleMinus:	return "TokDoubleMinus";
	case TokDoubleEqual:	return "TokDoubleEqual";
	case TokExclamEqual:	return "TokExclamEqual";

	case TokEqual:  	return "TokEqual";
	case TokPlusEqual:	return "TokPlusEqual";
	case TokMinusEqual:	return "TokMinusEqual";
	case TokStarEqual:	return "TokStarEqual";
	case TokSlashEqual:	return "TokSlashEqual";

	case TokRword_inline:	return "TokRword_inline";
	case TokRword_volatile:	return "TokRword_volatile";
	case TokRword_const:	return "TokRword_const"; 

	case TokRword_typedef:	return "TokRword_typedef";
	case TokRword_static:	return "TokRword_static";
	case TokRword_extern:	return "TokRword_extern";
	case TokRword_auto:	return "TokRword_auto";  
	case TokRword_register:	return "TokRword_register";

	case TokRword_struct:	return "TokRword_struct";
	case TokRword_enum:	return "TokRword_enum";
	case TokRword_union:	return "TokRword_union";

	case TokRword_void:	return "TokRword_void";
	case TokRword_char:	return "TokRword_char";
	case TokRword_short:	return "TokRword_short";
	case TokRword_int:	return "TokRword_int";
	case TokRword_long:	return "TokRword_long";
	case TokRword_signed:	return "TokRword_signed";
	case TokRword_unsigned:	return "TokRword_unsigned";
	case TokRword_float:	return "TokRword_float"; 
	case TokRword_double:	return "TokRword_double";
	case TokRword_Bool:	return "TokRword_Bool";  
	case TokRword_Complex:	return "TokRword_Complex";
        
	case TokRword_for:	return "TokRword_for";
	case TokRword_while:	return "TokRword_while";
	case TokRword_do:	return "TokRword_do";
	case TokRword_if:	return "TokRword_if";
	case TokRword_else:	return "TokRword_else";
	}
	return "BADENUM";
}

::std::ostream& operator<<(::std::ostream& os, enum eTokenType& tok)
{
	os << Token::enumname(tok);
	return os;
}

::std::ostream& operator<<(::std::ostream& os, struct Token& tok)
{
	os << Token::enumname(tok.type());
	return os;
}

