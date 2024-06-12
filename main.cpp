#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

#include "config.hpp"
#include "spl_lexer.hpp"
#include "spl_parser.hpp"
#include "tree.hpp"

char const* const HELP_SHORT_FLAG = "-h";
char const* const HELP_LONG_FLAG = "--help";
char const* const VERSION_SHORT_FLAG = "-V";
char const* const VERSION_LONG_FLAG = "--version";

extern std::FILE* yyin;
extern int yylex_destroy();

void print_help() {
  std::cout << "Usage: " << PROJECT_EXECUTABLE << " [options] INPUT_FILE\n";
  std::cout << "  -h, --help    Print this help message and exit.\n";
  std::cout << "  -V, --version Print the version of " << PROJECT_EXECUTABLE
            << " and exit." << std::endl;
}

void print_version() {
  std::cout << PROJECT_EXECUTABLE << " version " << PROJECT_VERSION
            << std::endl;
}

int main(int argc, char const* argv[]) {
  if (1 == argc) {
    std::cout << "Sample:\n";
    using namespace nd::spl::tree;
    std::shared_ptr<CompoundStatement> init =
        std::make_shared<CompoundStatement>(
            std::make_shared<AssignmentStatement>(
                std::make_shared<Identifier>("q"),
                std::make_shared<Constant>(0)),
            std::make_shared<AssignmentStatement>(
                std::make_shared<Identifier>("r"),
                std::make_shared<Identifier>("m")));
    std::shared_ptr<CompoundStatement> loop_body =
        std::make_shared<CompoundStatement>(
            std::make_shared<AssignmentStatement>(
                std::make_shared<Identifier>("q"),
                std::make_shared<CombinedOperands>(
                    std::make_shared<Identifier>("q"),
                    ArithmeticalOperator::PLUS, std::make_shared<Constant>(1))),
            std::make_shared<AssignmentStatement>(
                std::make_shared<Identifier>("r"),
                std::make_shared<CombinedOperands>(
                    std::make_shared<Identifier>("r"),
                    ArithmeticalOperator::MINUS,
                    std::make_shared<Identifier>("n"))));
    std::shared_ptr<WhileStatement> loop = std::make_shared<WhileStatement>(
        std::make_shared<BooleanExpression>(std::make_shared<Identifier>("r"),
                                            RelationalOperator::GREATER_EQUAL,
                                            std::make_shared<Identifier>("n")),
        loop_body);
  } else {
    if (0 == std::strncmp(argv[1], HELP_SHORT_FLAG, strlen(HELP_SHORT_FLAG)) ||
        0 == std::strncmp(argv[1], HELP_LONG_FLAG, strlen(HELP_LONG_FLAG))) {
      print_help();
      return EXIT_SUCCESS;
    } else if (0 == std::strncmp(argv[1], VERSION_SHORT_FLAG,
                                 strlen(VERSION_SHORT_FLAG)) ||
               0 == std::strncmp(argv[1], VERSION_LONG_FLAG,
                                 strlen(VERSION_LONG_FLAG))) {
      print_version();
      return EXIT_SUCCESS;
    } else {
      std::ifstream input_file{argv[1]};
      int exit_code;
      if (!input_file.good()) {
        exit_code = EXIT_FAILURE;
      } else if (input_file.eof()) {
        return EXIT_SUCCESS;
      } else {
        std::shared_ptr<nd::spl::tree::Program> prg_ptr;
        nd::spl::Lexer lexer{&input_file};
        nd::spl::Parser p{lexer, prg_ptr};
        if (p.parse() == 0 && prg_ptr != nullptr) {
          std::cout << *prg_ptr << std::endl;
          exit_code = EXIT_SUCCESS;
        } else {
          exit_code = EXIT_FAILURE;
        }
      }
      return exit_code;
    }
  }
}
