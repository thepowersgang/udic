/*
 */
#ifndef _PARSE_TYPES_HPP_
#define _PARSE_TYPES_HPP_

#include <types.hpp>
#include <preproc.hpp>
#include <ast.hpp>

extern TypeRef	parse_basetype(Preproc& lex, Program& program);
extern ::std::tuple<TypeRef,::std::string>	parse_fulltype(Preproc& lex, Program& program);

#endif
