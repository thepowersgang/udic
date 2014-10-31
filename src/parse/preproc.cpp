/*
 */
#include <common.hpp>
#include <preproc.hpp>
#include <parse_common.hpp>
#include <cassert>

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
				switch( tok.type() )
				{
				case TokInteger:
					throw ParseError::Todo("Preproc location annotation");
				case TokIdent:
					this->handle_preproc( tok.string() );
					break;
				default:
					throw ParseError::SyntaxError(FORMAT("Unexpected ",tok," expected integer/indent"));
				}
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

void Preproc::handle_preproc(::std::string tag)
{
	if( tag == "include" )
	{
		// 
		throw ParseError::Todo("Preprocessor - 'include'");
	}
	else
	{
		throw ParseError::SyntaxError(FORMAT("Unknown preprocessor op '", tag, "'"));
	}
}

