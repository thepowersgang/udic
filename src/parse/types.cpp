/*
 */
#include <common.hpp>
#include <parse_types.hpp>
#include <parse_common.hpp>

TypeRef	parse_basetype(Preproc& lex, Program& program, const bool expect_type)
{
	TypeRef	typeref;
	bool is_const = false;
	bool is_volatile = false;
	bool sign_seen = false;
	bool is_signed = true;
	bool size_seen = false;
	bool int_seen = false;
	auto size = IntClass::INT_INT;
	
	bool is_real = false;
	auto real_size = RealClass::REAL_FLOAT;
	bool is_complex = false;
	
	bool valid_type_token = true;
	while( valid_type_token )
	{
		Token tok = lex.get_token();
	
		switch( tok.type() )
		{
		case TokRword_const:	is_const = true;	break;
		case TokRword_volatile:	is_volatile = true;	break;
		
		case TokRword_unsigned:	sign_seen = true, is_signed = false;	break;
		case TokRword_signed:	sign_seen = true, is_signed = true;	break;
		
		case TokRword_char:	size_seen = true; size = IntClass::INT_CHAR;	break;
		case TokRword_int:	int_seen = true;	break;
		case TokRword_short:	size_seen = true; size = IntClass::INT_SHORT;	break;
		case TokRword_long:
			size_seen = true;
			switch(size)
			{
			case IntClass::INT_INT:  size = IntClass::INT_LONG;	break;
			case IntClass::INT_LONG: size = IntClass::INT_LONGLONG;	break;
			default:
				throw ParseError::Todo("Syntax error (long with invalid)");
			}
			break;
		
		case TokRword_float:	is_real = true; real_size = RealClass::REAL_FLOAT;	break;
		case TokRword_double:
			is_real = true;
			switch(size)
			{
			case IntClass::INT_INT:  real_size = RealClass::REAL_DOUBLE;     break;
			case IntClass::INT_LONG: real_size = RealClass::REAL_LONGDOUBLE; break;
			default:
				throw ParseError::Todo("Syntax error (double combined with integer size)");
			}
			break;
		case TokRword_Complex:
			is_complex = true;
			break;
		
		case TokRword_struct:	throw ParseError::Todo("'struct'");
		case TokRword_union:	throw ParseError::Todo("'union'");
		case TokRword_enum:	throw ParseError::Todo("'enum'");
		case TokRword_void:	throw ParseError::Todo("'void'");
		case TokIdent:
			if( typeref.get() || is_signed || size_seen || is_real || is_complex ) {
				valid_type_token = false;
				lex.put_back(tok);
			}
			else {
				throw ParseError::Todo("basetype ident");
			}
			break;
		default:
			valid_type_token = false;
			lex.put_back(tok);
			break;
		}
	}
	
	if( typeref.get() )
	{
		// Continue
	}
	else if( is_complex )
	{
		if( !is_real ) {
			throw ParseError::SyntaxError("Invalid use of '_Complex' without size");
		}
		typeref = TypeImpl::complex_(real_size);
	}
	else if( is_real )
	{
		typeref = TypeImpl::real(real_size);
	}
	else if( sign_seen || size_seen || int_seen )
	{
		typeref = TypeImpl::integer(size, is_signed);
	}
	else
	{
		throw ParseError::SyntaxError( FORMAT("No type provided, got ", lex.get_token()) );
	}
	
	if( is_const || is_volatile ) {
		throw ParseError::Todo("const/volatile");
	}
	
	return typeref;
}
::std::tuple<TypeRef,::std::string>	parse_fulltype(Preproc& lex, Program& program)
{
	throw ParseError::Todo("fulltype");
}

