%{
#include <cstdlib>
#include <memory>

#include "spl_lexer.hpp"

// Give Flex the prototype of yylex we want ...
#undef  YY_DECL
#define YY_DECL nd::spl::lalr::Parser::symbol_type nd::spl::lalr::Lexer::symbol_yylex()
// ... and declare it for the parser's sake.
%}

%option noyywrap
%option yyclass="nd::spl::lalr::Lexer"
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

"begin"               {return Parser::make__BEGIN();}
";"                   {return Parser::make_SEMICOLON();}
"end"                 {return Parser::make_END();}
"while"               {return Parser::make_WHILE();}
"do"                  {return Parser::make_DO();}
"od"                  {return Parser::make_OD();}
"if"                  {return Parser::make_IF();}
"then"                {return Parser::make_THEN();}
"else"                {return Parser::make_ELSE();}
"fi"                  {return Parser::make_FI();}
":="                  {return Parser::make_ASSIGN();}
"skip"                {return Parser::make_SKIP();}
"("                   {return Parser::make_PAR_OP();}
")"                   {return Parser::make_PAR_CL();}
"<="                  {return Parser::make_RELATIONAL_OPERATOR(tree::RelationalOperator::LESS_EQUAL);}
">="                  {return Parser::make_RELATIONAL_OPERATOR(tree::RelationalOperator::GREATER_EQUAL);}
"<"                   {return Parser::make_RELATIONAL_OPERATOR(tree::RelationalOperator::LESS_THAN);}
">"                   {return Parser::make_RELATIONAL_OPERATOR(tree::RelationalOperator::GREATER_THAN);}
"!="                  {return Parser::make_RELATIONAL_OPERATOR(tree::RelationalOperator::NOT_EQUAL);}
"="                   {return Parser::make_RELATIONAL_OPERATOR(tree::RelationalOperator::EQUAL);}
[+-]                  {using namespace std;if (0 == strncmp(yytext, "+", 1)){return Parser::make_ADDITIVE_OPERATOR(tree::ArithmeticalOperator::PLUS);}else{return Parser::make_ADDITIVE_OPERATOR(tree::ArithmeticalOperator::MINUS);}}
[*]                   {return Parser::make_MULTIPLICATIVE_OPERATOR(tree::ArithmeticalOperator::TIMES);}
[a-zA-Z][a-zA-Z0-9_]* {return Parser::make_IDENTIFIER(std::make_shared<tree::Identifier>(yytext));}
-?[1-9][0-9]*|0       {return Parser::make_INTEGER(std::make_shared<tree::Constant>(atoll(yytext)));}
.                     {return Parser::make_YYUNDEF();}
%%
