// prologe
%code requires {
#include <memory>
#include "tree.hpp"
}
%code provides {
// Give Flex the prototype of yylex we want ...
# define YY_DECL nd::spl::Parser::symbol_type yylex ()
// ... and declare it for the parser's sake.
YY_DECL;
}
%require "3.2"
%language "c++"
%define api.namespace {nd::spl}
%define api.parser.class {Parser}
%define api.value.type variant
%define api.token.constructor
// definitions
%token _BEGIN "begin"
%token SEMICOLON ";"
%token END "end"
%token WHILE "while"
%token DO "do"
%token OD "od"
%token IF "if"
%token THEN "then"
%token ELSE "else"
%token FI "fi"
%token ASSIGN ":="
%token SKIP "skip"
%token PAR_OP "("
%token PAR_CL ")"
%token <nd::spl::tree::RelationalOperator> RELATIONAL_OPERATOR
%token <nd::spl::tree::ArithmeticalOperator> ADDITIVE_OPERATOR
%token <nd::spl::tree::ArithmeticalOperator> MULTIPLICATIVE_OPERATOR
%token <std::shared_ptr<nd::spl::tree::Identifier>> IDENTIFIER
%token <std::shared_ptr<nd::spl::tree::Constant>> INTEGER
%nterm <std::shared_ptr<nd::spl::tree::Program>> program
%nterm <std::shared_ptr<nd::spl::tree::Statement>> statement
%nterm <std::shared_ptr<nd::spl::tree::CompoundStatement>> compound_statement
%nterm <std::shared_ptr<nd::spl::tree::WhileStatement>> while_statement
%nterm <std::shared_ptr<nd::spl::tree::IfStatement>> if_statement
%nterm <std::shared_ptr<nd::spl::tree::AssignmentStatement>> assignment_statement
%nterm <std::shared_ptr<nd::spl::tree::NullStatement>> null_statement
%nterm <std::shared_ptr<nd::spl::tree::BooleanExpression>> boolean_expression
%nterm <std::shared_ptr<nd::spl::tree::ArithmeticExpression>> arithmetic_expression
%nterm <std::shared_ptr<nd::spl::tree::Operand>> operand

%parse-param {std::shared_ptr<nd::spl::tree::Program>& result_ptr}
// rules
%%
program: statement { $program = std::make_shared<nd::spl::tree::Program>($statement); result_ptr = $program;};
statement: compound_statement { $statement = $compound_statement; }
| while_statement { $statement = $while_statement; }
| if_statement { $statement = $if_statement; }
| assignment_statement { $statement = $assignment_statement; }
| null_statement { $statement = $null_statement; }
;
compound_statement: _BEGIN statement[first] SEMICOLON statement[second] END { $compound_statement = std::make_shared<nd::spl::tree::CompoundStatement>($first, $second); };
while_statement: WHILE boolean_expression DO statement OD { $while_statement = std::make_shared<nd::spl::tree::WhileStatement>($boolean_expression, $statement); };
if_statement: IF boolean_expression THEN statement[true] ELSE statement[false] FI { $if_statement = std::make_shared<nd::spl::tree::IfStatement>($boolean_expression,$true,$false); };
assignment_statement: IDENTIFIER[id] ASSIGN arithmetic_expression { $assignment_statement = std::make_shared<nd::spl::tree::AssignmentStatement>($id, $arithmetic_expression); };
null_statement: SKIP { $null_statement = std::make_shared<nd::spl::tree::NullStatement>(); };
boolean_expression: arithmetic_expression[first] RELATIONAL_OPERATOR[op] arithmetic_expression[second] {$boolean_expression = std::make_shared<nd::spl::tree::BooleanExpression>($first,$op,$second);};
arithmetic_expression: operand {$arithmetic_expression = $operand;}
| ADDITIVE_OPERATOR[sign] operand {$arithmetic_expression = std::make_shared<nd::spl::tree::SignedOperand>($sign, $operand);}
| operand[first] ADDITIVE_OPERATOR[op] operand[second] {$arithmetic_expression = std::make_shared<nd::spl::tree::CombinedOperands>($first, $op, $second);}
| operand[first] MULTIPLICATIVE_OPERATOR[op] operand[second] {$arithmetic_expression = std::make_shared<nd::spl::tree::CombinedOperands>($first, $op, $second);}
;
operand: IDENTIFIER[id] {$operand = $id;}
| INTEGER[const] {$operand = $const;}
| PAR_OP arithmetic_expression PAR_CL {$operand = std::make_shared<nd::spl::tree::ParenthesizedArithmeticExpression>($arithmetic_expression);}
;
%%

// epilogue
void nd::spl::Parser::error(std::string const& message) {
	std::cerr << message << '\n';
}
