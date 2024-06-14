#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#ifdef WIN_IO_H
#include <io.h>
#define isatty(fd) _isatty(fd)
#endif

#include "spl_parser.hpp"

namespace nd::spl::lalr {
class Lexer : public yyFlexLexer {
 public:
  Lexer(std::istream* in) : yyFlexLexer(in) {}
  virtual ~Lexer() = default;

  virtual Parser::symbol_type symbol_yylex();
};
}  // namespace nd::spl::lalr
