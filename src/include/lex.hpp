/*
 */
#ifndef _LEX_H_
#define _LEX_H_

#include <iostream>
#include <vector>
extern "C" {
#include <stdint.h>
};
#include <types.hpp>

enum eTokenType
{
    TokNull,
	TokEOF,

	// - Preprocessor stuff
	TokNewline,
	TokWhitespace,
	TokComment,
	TokHash,
	TokDoubleHash,

	TokVerbatim,
	TokInclude,

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
	TokColon,
	TokQuestionMark,
	TokAt,  // Added for UDIC

    TokBackslash,
    TokBacktick,
    TokTilde,

	TokDoublePeriod,    // syntax error, always (needed due to quirk of lexer)
	TokTriplePeriod,    // varargs

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
	TokLessThanEqual,
	TokGreaterThanEqual,
	TokDoubleLessThan,
	TokDoubleGreaterThan,

	// - Assignment Operators
	TokEqual,
	TokPlusEqual,
	TokMinusEqual,
	TokStarEqual,
	TokSlashEqual,
	TokPercentEqual,
	TokPipeEqual,
	TokAmpersandEqual,

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

	TokRword_break,
	TokRword_continue,
	TokRword_goto,
	TokRword_return,
};

class Token
{
	enum eTokenType	m_type;
	::std::string	m_str;
	union {
		struct {
			uint64_t	val;
			IntClass	type;
		} integer;
		struct {
			double	value;
		} real;
	} m_data;

public:
	Token(enum eTokenType type);
	Token(enum eTokenType type, std::string value);
	static Token	integer(unsigned long long, IntClass::Size, bool);
	static Token	verbatim(::std::vector<Token> tokens);
	static Token	include(::std::string path, bool is_angle_string);

	static const char *typestr(enum eTokenType type);

	Token();
	~Token();
	enum eTokenType	type() const { return m_type; }
	::std::string	string() const { return m_str; }
};

extern ::std::ostream& operator<<(::std::ostream& os, struct Token& tok);
extern ::std::ostream& operator<<(::std::ostream& os, enum eTokenType& tok);

class Lexer
{
    struct EndOfFile {
    };

	::std::istream&	m_istream;
	bool	m_last_char_valid;
	char	m_last_char;
public:
	Lexer(::std::istream& is);

	Token get_token();
	::std::pair<::std::string,bool>	read_cpp_string();

private:
	char	getc();
	void	ungetc();

    signed int getSymbol();
	unsigned long long	read_number(unsigned int base);
	Token	parse_number(unsigned int base);

	uint32_t parseEscape(char enclosing);
};

#endif

