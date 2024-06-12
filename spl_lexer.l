%{
#include <cstdlib>
#include <memory>

#include "spl_lexer.hpp"

// Give Flex the prototype of yylex we want ...
#undef  YY_DECL
#define YY_DECL nd::spl::Parser::symbol_type nd::spl::Lexer::symbol_yylex()
// ... and declare it for the parser's sake.
%}

%option noyywrap
%option yyclass="nd::spl::Lexer"
%option c++
%s IN_COMMENT

%%
<INITIAL>{
"/*"                  BEGIN(IN_COMMENT);
}
<IN_COMMENT>{
"*/"                  BEGIN(INITIAL);
[^*\n]+               // eat comment in chunks
"*"                   // eat the lone star
\n                    yylineno++;
}
"//".*\n              {}
[ \t\r\n]             {}

"begin"               {return nd::spl::Parser::make__BEGIN();}
";"                   {return nd::spl::Parser::make_SEMICOLON();}
"end"                 {return nd::spl::Parser::make_END();}
"while"               {return nd::spl::Parser::make_WHILE();}
"do"                  {return nd::spl::Parser::make_DO();}
"od"                  {return nd::spl::Parser::make_OD();}
"if"                  {return nd::spl::Parser::make_IF();}
"then"                {return nd::spl::Parser::make_THEN();}
"else"                {return nd::spl::Parser::make_ELSE();}
"fi"                  {return nd::spl::Parser::make_FI();}
":="                  {return nd::spl::Parser::make_ASSIGN();}
"skip"                {return nd::spl::Parser::make_SKIP();}
"("                   {return nd::spl::Parser::make_PAR_OP();}
")"                   {return nd::spl::Parser::make_PAR_CL();}
"<="                  {return nd::spl::Parser::make_RELATIONAL_OPERATOR(nd::spl::tree::RelationalOperator::LESS_EQUAL);}
">="                  {return nd::spl::Parser::make_RELATIONAL_OPERATOR(nd::spl::tree::RelationalOperator::GREATER_EQUAL);}
"<"                   {return nd::spl::Parser::make_RELATIONAL_OPERATOR(nd::spl::tree::RelationalOperator::LESS_THAN);}
">"                   {return nd::spl::Parser::make_RELATIONAL_OPERATOR(nd::spl::tree::RelationalOperator::GREATER_THAN);}
"!="                  {return nd::spl::Parser::make_RELATIONAL_OPERATOR(nd::spl::tree::RelationalOperator::NOT_EQUAL);}
"="                   {return nd::spl::Parser::make_RELATIONAL_OPERATOR(nd::spl::tree::RelationalOperator::EQUAL);}
[+-]                  {using namespace std;if (0 == strncmp(yytext, "+", 1)){return nd::spl::Parser::make_ADDITIVE_OPERATOR(nd::spl::tree::ArithmeticalOperator::PLUS);}else{return nd::spl::Parser::make_ADDITIVE_OPERATOR(nd::spl::tree::ArithmeticalOperator::MINUS);}}
[*]                   {return nd::spl::Parser::make_MULTIPLICATIVE_OPERATOR(nd::spl::tree::ArithmeticalOperator::TIMES);}
[a-zA-Z][a-zA-Z0-9_]* {return nd::spl::Parser::make_IDENTIFIER(std::make_shared<nd::spl::tree::Identifier>(yytext));}
-?[1-9][0-9]*|0       {return nd::spl::Parser::make_INTEGER(std::make_shared<nd::spl::tree::Constant>(atoll(yytext)));}
.                     {return nd::spl::Parser::make_YYUNDEF();}
%%
