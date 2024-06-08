// prologe
%code requires {
#include <memory>
#include "tree.hpp"
}

%define api.value.type variant
// definitions
%token <nd::spl::tree::RelationalOperator> RELATIONAL_OPERATOR
%token <nd::spl::tree::Sign> SIGN
%token <nd::spl::tree::ArithmeticOperator> ARITHMETIC_OPERATOR
%token <std::shared_ptr<nd::spl::tree::Identifier>> IDENTIFIER
%token <std::shared_ptr<nd::spl::tree::Constant>> INTEGER
%nterm <std::shared_ptr<nd::spl::tree::Program>> program
%nterm <std::shared_ptr<nd::spl::tree::Statement>> statement
%nterm <std::shared_ptr<nd::spl::tree::CompoundStatement>> compound_statement
%nterm <std::shared_ptr<nd::spl::tree::WhileStatement>> while_statement
%nterm <std::shared_ptr<nd::spl::tree::IfStatement>> if_statement
%nterm <std::shared_ptr<nd::spl::tree::AssingmentStatement>> assignment_statement
%nterm <std::shared_ptr<nd::spl::tree::NullStatement>> null_statement
%nterm <std::shared_ptr<nd::spl::tree::BooleanExpression>> boolean_expression
%nterm <std::shared_ptr<nd::spl::tree::ArithmeticExpression>> arithmetic_expression
%nterm <std::shared_ptr<nd::spl::tree::Operand>> operand
%%
// rules
program = statement { $program = std::make_shared<nd::spl::tree::Program>($statement); };
statement = compound_statement { $statement = $compound_statement; }
| while_statement { $statement = $while_statement; }
| if_statement { $statement = $if_statement; }
| assignment_statement { $statement = $assignment_statement; }
| null_statement { $statement = $null_statement; }
;
compound_statement = "begin" statement[first] ';' statement[second] "end" { $compund_statement = std::make_shared<nd::spl::tree::CompundStatement>($first, $second); };
while_statement = "while" boolean_expression "do" statement "od" { $while_statement = std::make_shared<nd::spl::tree::WhileStatement>($boolean_expression, $statement); };
if_statement = "if" boolean_expression "then" statement[true] "else" statement[false] "fi" { $if_statement = std::make_shared<nd::spl::tree::IfStatement($boolean_expression,$true,$false); };
assigment_statement = identifier ":=" arithmentic_expression { $assignment_statement = std::make_shared<nd::spl::tree::AssignmentStatement>($identifier, $arithmetic_expression); };
null_statement = "skip" { $null_statement = std::make_shared<nd::spl::tree::NullStatement>(); };
boolean_expression = arithmetic_expression[first] RELATIONAL_OPERATOR[op] arithmetic_expression[second] {$boolean_expression = std::make_shared<nd::spl::tree::BooleanExpression>($first,$op,$second);};
arithmetic_expression = operand {$arithmetic_expression = $operand;}
| SIGN operand {$arithmetic_expression = std::make_shared<SignedOperand>($SIGN, $operand);}
| operand[first] ARITHMETIC_OPERATOR[op] operand[second] {$arithmetic_expression = std::make_shared<CombinedOperands>($first, $op, $second);}
;
operand = IDENTIFIER[id] {$operand = $id;}
| INTEGER[const] {$operand = $const;}
| '(' arithmetic_expression ')' {$operand = std::make_shared<ParenthesizedArithmeticExpression>($arithmetic_expression);}
;
%%

// epilogue
