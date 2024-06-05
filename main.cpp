#include <cstring>
#include <iostream>

#include "config.hpp"

char const* const HELP_SHORT_FLAG = "-h";
char const* const HELP_LONG_FLAG = "--help";
char const* const VERSION_SHORT_FLAG = "-V";
char const* const VERSION_LONG_FLAG = "--version";

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
    print_help();
    return EXIT_FAILURE;
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
			return EXIT_FAILURE;
    }
  }
}
