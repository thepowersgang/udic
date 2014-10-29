/*
 */
#include <preproc.hpp>
#include <ast.hpp>
#include <parse_common.hpp>
#include <parse_types.hpp>

void parse_typedef(Preproc& lex, Program& program)
{
	TypeRef	base = parse_basetype(lex, program);
	throw ParseError::Todo("typedef");
}

::std::vector<Definition> parse_definition(Preproc& lex, Program& program)
{
	// Get storage class
	TypeRef	base = parse_basetype(lex, program);
	
	::std::string	name;
	TypeRef	type;
	::std::tie(type,name) = parse_fulltype(lex, program);
	
	throw ParseError::Todo("definition");
}

Program parse_root(::std::istream& is)
{
	Program	program;
	Preproc	lex(is);
	
	for( ;; )
	{
		Token tok = lex.get_token();
		switch( tok.type() )
		{
		case TokEOF:
			return program;
		case TokRword_typedef:
			parse_typedef(lex, program);
			break;
		default:
			lex.put_back(tok);
			program.append_definitions( parse_definition(lex, program) );
			break;
		}
	}
}

