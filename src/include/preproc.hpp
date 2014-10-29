/*
 */
#ifndef _PREPROC_HPP_
#define _PREPROC_HPP_

#include <lex.hpp>

class Preproc
{
	Lexer	m_root_lexer;
	bool	m_cached_valid;
	Token	m_cached;
public:
	Preproc(::std::istream& is);
	
	Token	get_token();
	void	put_back(Token tok);
	
private:
	Token	_get_token();
};

#endif
