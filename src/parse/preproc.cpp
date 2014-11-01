/*
 * UDI C Converter
 * - By John Hodge (thePowersGang)
 * 
 * parse/preproc.cpp
 * - Rudimentary handling of the C preprocessor
 */
#include <common.hpp>
#include <preproc.hpp>
#include <parse_common.hpp>
#include <cassert>
#include <vector>

Preproc::Preproc(::std::istream& is, const char *filename):
	m_root_lexer(is),
	m_cached_valid(false),
	m_cur_filename(filename),
	m_cur_linenum(1)
{
}

Token Preproc::_get_token()
{
	return m_root_lexer.get_token();
}

Token Preproc::get_token()
{
	if( m_cached_valid )
	{
		m_cached_valid = false;
		return m_cached;
	}
	else
	{
		for(;;)
		{
			Token tok = _get_token();
			switch( tok.type() )
			{
			case TokHash:
				// Preprocessor stuff!
				tok = _get_token();
				while( tok.type() == TokWhitespace )
					tok = _get_token();
				switch( tok.type() )
				{
				case TokInteger:
					throw ParseError::Todo("Preproc location annotation");
				case TokIdent:
					return this->handle_preproc( tok.string() );
				default:
					throw ParseError::SyntaxError(FORMAT("Unexpected ",tok," expected integer/indent"));
				}
				break;
			case TokWhitespace:
				break;
			case TokNewline:
				break;
			case TokComment:
				break;
			default:
				return tok;
			}
		}
	}
}

void Preproc::put_back(Token tok)
{
	assert(!m_cached_valid);
	m_cached = tok;
}

Token Preproc::handle_preproc(::std::string tag)
{
	Token tok = _get_token();
	if(tok.type() != TokWhitespace)
		throw ParseError::SyntaxError(FORMAT("Preprocessor expected whitespace after name, got ", tok));
	
	if( tag == "include" )
	{
		// 
		throw ParseError::Todo("Preprocessor - 'include'");
	}
	else if( tag == "define" )
	{
		// "#define"
		::std::vector<Token>	items;
		while( (tok = _get_token()).type() != TokNewline )
			items.push_back(tok);
		return Token::verbatim(items);
	}
	else
	{
		throw ParseError::SyntaxError(FORMAT("Unknown preprocessor op '", tag, "'"));
	}
}

