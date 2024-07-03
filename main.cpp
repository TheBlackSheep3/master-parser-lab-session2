#include <fstream>
#include <iostream>
#include <memory>
#include <utility>

#include <argparse/argparse.hpp>

#include "config.hpp"
#include "container.hpp"
#include "spl_lexer.hpp"
#include "spl_parser.hpp"
#include "tree.hpp"

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
  argparse::ArgumentParser program(PROJECT_EXECUTABLE, PROJECT_VERSION);
  program.add_argument("-o", "--output")
      .help("write output to OUTPUT_FILE")
      .metavar("OUTPUT_FILE");
  program.add_argument("-f", "--file")
      .help("read input from INPUT_FILE")
      .metavar("INPUT_FILE");
  program.add_description(
      "Parse code of the sample programming language, as described in "
      "semantics of programming languages, and put out a tree representing the "
      "program structure.");

  try {
    program.parse_args(argc, argv);
  } catch (std::exception const& e) {
    std::cerr << e.what() << '\n' << program;
    exit(EXIT_FAILURE);
  }

  Container<std::istream> input_ptr;
  Container<std::ostream> output_ptr;
  if (auto input_file = program.present("--file")) {
    input_ptr = std::move(
        Container<std::istream>(new std::ifstream{*input_file}, true));
  } else {
    input_ptr = std::move(Container<std::istream>(&std::cin, false));
  }
  if (auto output_file = program.present("--output")) {
    output_ptr = std::move(
        Container<std::ostream>(new std::ofstream{*output_file}, true));
  } else {
    output_ptr = std::move(Container<std::ostream>(&std::cout, false));
  }

  return parse(*input_ptr, *output_ptr);
}
