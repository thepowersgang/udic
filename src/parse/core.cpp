/*
 */
#include <preproc.hpp>
#include <ast.hpp>
#include <parse_common.hpp>
#include <parse_types.hpp>

void parse_typedef(Preproc& lex, Program& program)
{
	TypeRef	base = parse_basetype(lex, program, true);

	::std::string	name;
	TypeRef	type;
	::std::tie(type,name) = parse_fulltype(lex, program, base);

    program.add_typedef(name, type);
    if( lex.get_token().type() != TokSemicolon )
        throw ParseError::SyntaxError("Expected TokSemicolon after typedef");
}

::std::vector<Definition> parse_definition(Preproc& lex, Program& program)
{
	// TODO: Get storage class
	Token tok = lex.get_token();
	switch(tok.type())
	{
	case TokVerbatim:
		return ::std::vector<Definition>();
		//throw ParseError::Todo("Verbatim definition");
	case TokRword_extern:
		throw ParseError::Todo("Extern storage class");
	case TokRword_inline:
		throw ParseError::Todo("'inline' storage class");
	case TokRword_static:
		throw ParseError::Todo("'static' storage class");
	case TokRword_auto:
		throw ParseError::Todo("'auto' storage class");
	default:
		lex.put_back(tok);
		break;
	}

	// Get main type
	TypeRef	base = parse_basetype(lex, program, true);

	::std::string	name;
	TypeRef	type;
	::std::tie(type,name) = parse_fulltype(lex, program, base);

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
		case TokInclude:
			// #include
			// - Search list of known headers (udi headers)
			//  > If found, mark the contained functions as valid?
			// - Otherwise, ignore and pass
			//program.append_definitions( (::std::vector<Definition>){ Definition("#include <filename>") } );
			break;
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

