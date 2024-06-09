#pragma once

#include <memory>
#include <ostream>
#include <string>

namespace nd::spl::tree {
class INode {
 public:
  virtual ~INode() = default;
  virtual std::string get_str() const = 0;
};

class Statement;
class Program : public INode {
 public:
  Program(std::shared_ptr<Statement> const& statement)
      : m_statement{statement} {}
  virtual ~Program() = default;
  std::string get_str() const override;

 private:
  std::shared_ptr<Statement> m_statement;
};

class Statement : public INode {
 public:
  virtual ~Statement() = default;
};

class CompoundStatement : public Statement {
 public:
  CompoundStatement(std::shared_ptr<Statement> const& first,
                    std::shared_ptr<Statement> const& second) {
    m_statements[0] = std::shared_ptr(first);
    m_statements[1] = std::shared_ptr(second);
  }
  virtual ~CompoundStatement() = default;
  std::string get_str() const override;

 private:
  std::shared_ptr<Statement> m_statements[2];
};

class BooleanExpression;

class WhileStatement : public Statement {
 public:
  WhileStatement(std::shared_ptr<BooleanExpression> const& condition,
                 std::shared_ptr<Statement> const& body)
      : m_condition{condition}, m_body{body} {}
  virtual ~WhileStatement() = default;
  std::string get_str() const override;

 private:
  std::shared_ptr<BooleanExpression> m_condition;
  std::shared_ptr<Statement> m_body;
};

class IfStatement : public Statement {
 public:
  IfStatement(std::shared_ptr<BooleanExpression> const& condition,
              std::shared_ptr<Statement> const& true_block,
              std::shared_ptr<Statement> const& false_block)
      : m_condition{condition} {
    m_blocks[0] = std::shared_ptr(true_block);
    m_blocks[1] = std::shared_ptr(false_block);
  }
  virtual ~IfStatement() = default;
  std::string get_str() const override;

 private:
  std::shared_ptr<BooleanExpression> m_condition;
  std::shared_ptr<Statement> m_blocks[2];
};

class Identifier;
class ArithmeticExpression;

class AssignmentStatement : public Statement {
 public:
  AssignmentStatement(
      std::shared_ptr<Identifier> const& identifier,
      std::shared_ptr<ArithmeticExpression> const& arithmeticExpression)
      : m_identifier{identifier},
        m_arithmeticExpression{arithmeticExpression} {}
  virtual ~AssignmentStatement() = default;
  std::string get_str() const override;

 private:
  std::shared_ptr<Identifier> m_identifier;
  std::shared_ptr<ArithmeticExpression> m_arithmeticExpression;
};

class NullStatement : public Statement {
 public:
  NullStatement() = default;
  virtual ~NullStatement() = default;
  std::string get_str() const override;
};

enum class RelationalOperator {
  LESS_THAN,
  LESS_EQUAL,
  EQUAL,
  GREATER_EQUAL,
  GREATER_THAN,
  NOT_EQUAL
};

class BooleanExpression : public INode {
 public:
  BooleanExpression(std::shared_ptr<ArithmeticExpression> const& first,
                    RelationalOperator op,
                    std::shared_ptr<ArithmeticExpression> const& second)
      : m_operator{op} {
    m_arithmethicExpressions[0] = std::shared_ptr(first);
    m_arithmethicExpressions[1] = std::shared_ptr(second);
  }
  virtual ~BooleanExpression() = default;
  std::string get_str() const override;

 private:
  std::shared_ptr<ArithmeticExpression> m_arithmethicExpressions[2];
  RelationalOperator m_operator;
};

class ArithmeticExpression : public INode {
 public:
  virtual ~ArithmeticExpression() = default;
};

class Operand;

enum class ArithmeticalOperator { PLUS, MINUS, TIMES };

class SignedOperand : public ArithmeticExpression {
 public:
  SignedOperand(ArithmeticalOperator sign,
                std::shared_ptr<Operand> const& operand)
      : m_sign{sign}, m_operand{operand} {
    switch (sign) {
      case nd::spl::tree::ArithmeticalOperator::PLUS:
      case nd::spl::tree::ArithmeticalOperator::MINUS:
        break;
      default:
        throw std::invalid_argument("m_sign");
    }
  }
  virtual ~SignedOperand() = default;
  std::string get_str() const override;

 private:
  ArithmeticalOperator m_sign;
  std::shared_ptr<Operand> m_operand;
};

class CombinedOperands : public ArithmeticExpression {
 public:
  CombinedOperands(std::shared_ptr<Operand> const& first,
                   ArithmeticalOperator op,
                   std::shared_ptr<Operand> const& second)
      : m_operator{op} {
    m_operands[0] = std::shared_ptr(first);
    m_operands[1] = std::shared_ptr(second);
  }
  virtual ~CombinedOperands() = default;
  std::string get_str() const override;

 private:
  ArithmeticalOperator m_operator;
  std::shared_ptr<Operand> m_operands[2];
};

class Operand : public ArithmeticExpression {
 public:
  virtual ~Operand() = default;
};

class Identifier : public Operand {
 public:
  Identifier(std::string const& id) : m_id{id} {}
  Identifier(char* const id) : m_id{id} {}
  virtual ~Identifier() = default;
  std::string get_str() const override;

 private:
  std::string m_id;
};

class Constant : public Operand {
 public:
  Constant(long long value) : m_value{value} {}
  virtual ~Constant() = default;
  std::string get_str() const override;

 private:
  long long m_value;
};

class ParenthesizedArithmeticExpression : public Operand {
 public:
  ParenthesizedArithmeticExpression(
      std::shared_ptr<ArithmeticExpression> const& expression)
      : m_expression{expression} {}
  virtual ~ParenthesizedArithmeticExpression() = default;
  std::string get_str() const override;

 private:
  std::shared_ptr<ArithmeticExpression> m_expression;
};
}  // namespace nd::spl::tree

std::ostream& operator<<(std::ostream& ostream,
                         nd::spl::tree::INode const& node);
