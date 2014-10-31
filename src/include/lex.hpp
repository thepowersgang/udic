/*
 */
#ifndef _LEX_H_
#define _LEX_H_

#include <iostream>
extern "C" {
#include <stdint.h>
};
#include <types.hpp>

enum eTokenType
{
	TokEOF,
	
	// - Preprocessor stuff
	TokNewline,
	TokComment,
	TokHash,
	
	// - Leaf nodes
	TokIdent,
	TokInteger,
	TokReal,

	// - Groupings
	TokBraceOpen, TokBraceClose,
	TokParenOpen, TokParenClose,
	TokSquareOpen, TokSquareClose,
	TokLessThan, TokGreaterThan,
	
	// - Misc
	TokPeriod, TokAmpersand,
	TokComma, TokSemicolon,
	TokStar,
	TokArrow,	// ->
	
	// - Operators
	TokPlus, TokMinus,
	TokSlash, TokPercent,
	TokPipe,
	TokCaret,
	TokExclam,
	TokDoubleAmpersand, TokDoublePipe,
	TokDoublePlus,	// ungood
	TokDoubleMinus,
	TokDoubleEqual,
	TokExclamEqual,
	
	// - Assignment Operators
	TokEqual,
	TokPlusEqual,
	TokMinusEqual,
	TokStarEqual,
	TokSlashEqual,
	
	TokRword_inline,
	TokRword_volatile,
	TokRword_const,
	
	TokRword_typedef,
	TokRword_static,
	TokRword_extern,
	TokRword_auto,
	TokRword_register,
	
	TokRword_struct,
	TokRword_enum,
	TokRword_union,
	
	TokRword_void,
	TokRword_char,
	TokRword_short,
	TokRword_int,
	TokRword_long,
	TokRword_signed,
	TokRword_unsigned,
	TokRword_float,
	TokRword_double,
	TokRword_Bool,
	TokRword_Complex,

	TokRword_for,
	TokRword_while,
	TokRword_do,
	TokRword_if,
	TokRword_else,
};

struct Token
{
	enum eTokenType	m_type;
	::std::string	m_stringval;
	union {
		struct {
			uint64_t	val;
			IntClass	type;
		} integer;
		struct {
			double	value;
		} real;
	} m_data;
	
	static Token	eof();
	static Token	single(enum eTokenType type);
	static Token	string(enum eTokenType, std::string value);
	
	static const char *enumname(enum eTokenType type);
	
	Token();
	~Token();
	enum eTokenType	type() const { return m_type; }
	::std::string	string() const { return m_stringval; }
private:
	Token(enum eTokenType type);
};

extern ::std::ostream& operator<<(::std::ostream& os, struct Token& tok);
extern ::std::ostream& operator<<(::std::ostream& os, enum eTokenType& tok);

class Lexer
{
	::std::istream&	m_is;
	bool	m_cached_valid;
	char	m_cached;
public:
	Lexer(::std::istream& is);
	
	Token get_token();

private:
	char	_getc();
	void	_ungetc();
};

#endif

