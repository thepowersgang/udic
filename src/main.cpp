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
		if( !fp.is_open() )
            throw ParseError::Todo("enoent");
		return parse_root(fp, filename);
	}
}

::std::vector<const char*> parse_args(int argc, char* argv[])
{
    ::std::vector<const char*>    free;
    bool    force_free = false;

    for( int i = 1; i < argc; i ++ )
    {
        const char* arg = argv[i];
        if( arg[0] != '-' or force_free )
        {
            // Free argument, onto the list with you
            free.push_back(arg);
        }
        else if( arg[1] != '-' )
        {
            // Single character arguments

        }
        else if( arg[2] != '\0' )
        {
            // Long arguments
        }
        else
        {
            // end of argument list, everything afterwards is free
            force_free = true;
        }
    }

    return free;
}

int main(int argc, char *argv[])
{
    ::std::vector<const char*> files = parse_args(argc, argv);
    if( files.size() == 0 ) {
        ::std::cerr << "Please pass a file to process" << ::std::endl;
        return 1;
    }
	try
	{
		Program	ast = parse(files[0]);
	}
	catch( const ParseError::Base& e )
	{
		::std::cerr << "Parse Error: " << e.what() << ": " << e.message() << ::std::endl;
	}

	return 0;
}
