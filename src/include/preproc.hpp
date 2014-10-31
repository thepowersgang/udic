/*
 */
#ifndef _PREPROC_HPP_
#define _PREPROC_HPP_

#include <string>
#include <lex.hpp>

class Preproc
{
	Lexer	m_root_lexer;
	bool	m_cached_valid;
	Token	m_cached;
	
	::std::string	m_cur_filename;
	 int	m_cur_linenum;
public:
	Preproc(::std::istream& is, const char *filename);
	
	Token	get_token();
	void	put_back(Token tok);
	
private:
	Token	_get_token();
	void	handle_preproc(::std::string tag);
};

#endif
