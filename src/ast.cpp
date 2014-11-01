/*
 */
#include <ast.hpp>

Program::Program()
{
}

void Program::append_definitions(::std::vector<Definition> new_defs)
{
	::std::move(new_defs.begin(), new_defs.end(), ::std::back_inserter(m_definitions));
}

