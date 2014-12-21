/*
 */
#include <ast.hpp>
#include <parse_common.hpp>

Program::Program()
{
}

TypeRef Program::define_struct(::std::string name, ::std::vector<Composite::item_t> items)
{
    if(name == "")
    {
        ::std::auto_ptr<Composite>  str(new Composite(Composite::TagStruct(), items));

        TypeRef ret(*str);

        m_anon_structs.emplace_back( str );

        return ret;
    }
    else
    {
        auto rv = m_named_structs.insert( ::std::make_pair(name, Composite(Composite::TagStruct(), items)) );
        if( rv.second == false )
        {
            throw ParseError::Todo("redefinition");
        }
        return TypeRef(rv.first->second);
    }
}
TypeRef Program::get_struct(::std::string name)
{
    throw ParseError::Todo("Program::get_struct");
}

void Program::add_typedef(::std::string name, TypeRef type)
{
    auto rv = m_typedefs.insert( ::std::make_pair(name, type) );
    if( rv.second == false )
        throw ParseError::Todo("redefinition");
}

void Program::append_definitions(::std::vector<Definition> new_defs)
{
	::std::move(new_defs.begin(), new_defs.end(), ::std::back_inserter(m_definitions));
}

