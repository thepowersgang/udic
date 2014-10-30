/*
 */
#include <iostream>
#include <fstream>
#include <parse_core.hpp>
#include <parse_common.hpp>
#include <cstring>

Program parse(const char *filename)
{
	if( ::std::strcmp(filename, "-") == 0 )
	{
		return parse_root(::std::cin, filename);
	}
	else
	{
		::std::ifstream	fp(filename);
		return parse_root(fp, filename);
	}
}

int main(int argc, char *argv[])
{
	try
	{
		Program	ast = parse(argv[1]);
	}
	catch( const ParseError::Base& e )
	{
		::std::cerr << "Parse Error: " << e.what() << ": " << e.message() << ::std::endl;
	}
	
	return 0;
}

