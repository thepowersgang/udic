/*
 */
#include <preproc.hpp>
#include <ast.hpp>
#include <parse_common.hpp>
#include <parse_types.hpp>

void parse_typedef(Preproc& lex, Program& program)
{
	TypeRef	base = parse_basetype(lex, program, true);
	throw ParseError::Todo("typedef");
}

::std::vector<Definition> parse_definition(Preproc& lex, Program& program)
{
	// TODO: Get storage class
	
	// Get main type
	TypeRef	base = parse_basetype(lex, program, true);
	
	::std::string	name;
	TypeRef	type;
	::std::tie(type,name) = parse_fulltype(lex, program);
	
	throw ParseError::Todo("definition");
}

Program parse_root(::std::istream& is, const char *filename)
{
	Program	program;
	Preproc	lex(is, filename);
	
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

