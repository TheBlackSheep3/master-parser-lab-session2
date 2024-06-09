#include <ostream>
#include <string>
#include <stdexcept>

#include "tree.hpp"

std::ostream& operator<<(std::ostream& ostream,
                         nd::spl::tree::INode const& node) {
  return ostream << node.get_str();
}

std::string nd::spl::tree::Program::get_str() const {
  return std::string("PROGRAM(" + m_statement->get_str() + ")");
}

std::string nd::spl::tree::CompoundStatement::get_str() const {
  return std::string("COMP_STMT(" + m_statements[0]->get_str() + "," +
                     m_statements[1]->get_str() + ")");
}

std::string nd::spl::tree::WhileStatement::get_str() const {
  return std::string("WHILE_STMT(" + m_condition->get_str() + "," +
                     m_body->get_str() + ")");
}

std::string nd::spl::tree::IfStatement::get_str() const {
  return std::string("IF_STMT(" + m_condition->get_str() + "," +
                     m_blocks[0]->get_str() + "," + m_blocks[1]->get_str() +
                     ")");
}

std::string nd::spl::tree::AssignmentStatement::get_str() const {
  return std::string("ASSING_STMT(" + m_identifier->get_str() + "," +
                     m_arithmeticExpression->get_str() + ")");
}

std::string nd::spl::tree::NullStatement::get_str() const {
  return std::string("NULL_STMT");
}

std::string nd::spl::tree::BooleanExpression::get_str() const {
  std::string op;
  switch (m_operator) {
    case nd::spl::tree::RelationalOperator::LESS_THAN:
      op = std::string("<");
      break;
    case nd::spl::tree::RelationalOperator::LESS_EQUAL:
      op = std::string("<=");
      break;
    case nd::spl::tree::RelationalOperator::EQUAL:
      op = std::string("=");
      break;
    case nd::spl::tree::RelationalOperator::GREATER_EQUAL:
      op = std::string(">=");
      break;
    case nd::spl::tree::RelationalOperator::GREATER_THAN:
      op = std::string(">");
      break;
    case nd::spl::tree::RelationalOperator::NOT_EQUAL:
      op = std::string("!=");
      break;
  }
  return std::string("B_EXPR(" + m_arithmethicExpressions[0]->get_str() + "," +
                     op + "," + m_arithmethicExpressions[1]->get_str() + ")");
}

std::string nd::spl::tree::SignedOperand::get_str() const {
  std::string sign;
  switch (m_sign) {
    case nd::spl::tree::ArithmeticalOperator::PLUS:
      sign = std::string("+");
      break;
    case nd::spl::tree::ArithmeticalOperator::MINUS:
      sign = std::string("-");
      break;
    default:
      throw std::invalid_argument("m_sign");
  }
  return std::string("SIGN_OP(" + sign + "," + m_operand->get_str() + ")");
}

std::string nd::spl::tree::CombinedOperands::get_str() const {
  std::string op;
  switch (m_operator) {
    case nd::spl::tree::ArithmeticalOperator::PLUS:
      op = std::string("+");
      break;
    case nd::spl::tree::ArithmeticalOperator::MINUS:
      op = std::string("-");
      break;
    case nd::spl::tree::ArithmeticalOperator::TIMES:
      op = std::string("*");
      break;
  }
  return std::string("AR_EXPR(" + m_operands[0]->get_str() + "," + op + "," +
                     m_operands[1]->get_str() + ")");
}

std::string nd::spl::tree::Identifier::get_str() const {
  return std::string("ID(" + m_id + ")");
}

std::string nd::spl::tree::Constant::get_str() const {
  return std::string("CONST(" + std::to_string(m_value) + ")");
}

std::string nd::spl::tree::ParenthesizedArithmeticExpression::get_str() const {
  return std::string("PAREN_EXPR(" + m_expression->get_str() + ")");
}
