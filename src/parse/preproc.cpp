/*
 */
#include <preproc.hpp>
#include <cassert>

Preproc::Preproc(::std::istream& is):
	m_root_lexer(is),
	m_cached_valid(false)
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

