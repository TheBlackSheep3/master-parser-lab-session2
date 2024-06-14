#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include "config.hpp"
#include "spl_lexer.hpp"
#include "spl_parser.hpp"
#include "parameters.hpp"
#include "tree.hpp"

void print_help() {
  std::cout << "Usage: " << PROJECT_EXECUTABLE << " [options] [INPUT_FILE]\n";
  std::cout << "  -o, --output OUTPUT_FILE   Write output to OUTPUT_FILE.\n";
  std::cout
      << "  -h, --help                 Print this help message and exit.\n";
  std::cout << "  -V, --version              Print the version of "
            << PROJECT_EXECUTABLE << " and exit.\n\n";
  std::cout << "If no INPUT_FILE is provided, input is read from stdin.\n";
}

void print_version() {
  std::cout << PROJECT_EXECUTABLE << " version " << PROJECT_VERSION
            << std::endl;
}

int parse(std::istream& input, std::ostream& output) {
  if (!input.good()) {
    return EXIT_FAILURE;
  } else if (input.eof()) {
    return EXIT_SUCCESS;
  } else {
    std::shared_ptr<nd::spl::tree::Program> prg_ptr;
    nd::spl::lalr::Lexer lexer{&input};
    nd::spl::lalr::Parser p{lexer, prg_ptr};
    if (p.parse() == 0 && prg_ptr != nullptr) {
      output << *prg_ptr << std::endl;
      return EXIT_SUCCESS;
    } else {
      return EXIT_FAILURE;
    }
  }
}

int main(int argc, char const* argv[]) {
  Parameters params{/* ignore binary name */ argv + 1, argv + argc};
  if (params.help_requested()) {
    print_help();
    return EXIT_SUCCESS;
  } else if (params.version_requested()) {
    print_version();
    return EXIT_SUCCESS;
  } else {
    Container<std::istream> input_ptr;
    Container<std::ostream> output_ptr;
    if (params.input_file().empty()) {
      input_ptr = std::move(Container<std::istream>(&std::cin, false));
    } else {
      input_ptr = std::move(Container<std::istream>(
          new std::ifstream{params.input_file()}, true));
    }
    if (!params.output_requested()) {
      output_ptr = std::move(Container<std::ostream>(&std::cout, false));
    } else {
      if (!params.output_file().empty()) {
        output_ptr = std::move(Container<std::ostream>(
            new std::ofstream{params.output_file()}, true));
      } else {
        std::cerr << "missing parameter OUTPUT_FILE\n\nRun '"
                  << PROJECT_EXECUTABLE << " " << HELP_LONG_FLAG
                  << "' for more information.\n";
        return EXIT_FAILURE;
      }
    }
    return parse(*input_ptr, *output_ptr);
  }
}
