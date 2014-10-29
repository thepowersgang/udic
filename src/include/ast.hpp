/*
 */
#ifndef _AST_HPP_
#define _AST_HPP_

#include <types.hpp>
#include <map>
#include <vector>

class ASTNodeImpl;

typedef ::std::unique_ptr<ASTNodeImpl>	ASTNode;

struct Definition
{
	::std::string	m_name;
	TypeRef	m_type;
	ASTNode	m_value;
};

class Program
{
	// Typedefs
	::std::map< ::std::string, TypeRef >	m_typedefs;
	
	// Definitions
	::std::vector< Definition >	m_definitions;

public:
	Program();
	
	void append_definitions(::std::vector<Definition> new_defs);
};

class ASTNodeImpl
{
	
};

#endif
