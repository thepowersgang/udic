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
    ::std::map< ::std::string, Composite>   m_named_structs;
    ::std::vector< ::std::auto_ptr<Composite> > m_anon_structs;

	// Typedefs
	::std::map< ::std::string, TypeRef >	m_typedefs;

	// Definitions
	::std::vector< Definition >	m_definitions;

public:
	Program();

    TypeRef define_struct(::std::string name, ::std::vector<Composite::item_t> items);
    TypeRef get_struct(::std::string name);

    void add_typedef(::std::string name, TypeRef type);

	void append_definitions(::std::vector<Definition> new_defs);
};

class ASTNodeImpl
{

};

#endif
