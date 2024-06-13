#pragma once

#include <cstring>
#include <string>

char const* const HELP_SHORT_FLAG = "-h";
char const* const HELP_LONG_FLAG = "--help";
char const* const VERSION_SHORT_FLAG = "-V";
char const* const VERSION_LONG_FLAG = "--version";
char const* const OUTPUT_SHORT_FLAG = "-o";
char const* const OUTPUT_LONG_FLAG = "--output";

class Parameters {
 public:
  Parameters(char const* const* it_begin, char const* const* it_end) {
    for (auto curr = it_begin; curr < it_end; ++curr) {
      if (!curr) {
        continue;
      } else if (0 == std::strncmp(*curr, HELP_SHORT_FLAG,
                                   strlen(HELP_SHORT_FLAG)) ||
                 0 == std::strncmp(*curr, HELP_LONG_FLAG,
                                   strlen(HELP_LONG_FLAG))) {
        m_help = true;
      } else if (0 == std::strncmp(*curr, VERSION_SHORT_FLAG,
                                   strlen(VERSION_SHORT_FLAG)) ||
                 0 == std::strncmp(*curr, VERSION_LONG_FLAG,
                                   strlen(VERSION_LONG_FLAG))) {
        m_version = true;
      } else if (0 == std::strncmp(*curr, OUTPUT_SHORT_FLAG,
                                   strlen(OUTPUT_SHORT_FLAG)) ||
                 0 == std::strncmp(*curr, OUTPUT_LONG_FLAG,
                                   strlen(OUTPUT_LONG_FLAG))) {
        m_output = true;
      } else if (m_output && m_output_file.empty()) {
        m_output_file = std::string{*curr};
      } else {
        m_input_file = std::string(*curr);
      }
    }
  }

  bool help_requested() const { return m_help; }
  bool version_requested() const { return m_version; }
  bool output_requested() const { return m_output; }
  std::string const& output_file() const { return m_output_file; }
  std::string const& input_file() const { return m_input_file; }

 private:
  bool m_help = false;
  bool m_version = false;
  bool m_output = false;
  std::string m_output_file;
  std::string m_input_file;
};

template <typename T>
class Container {
 public:
  Container() : Container(nullptr, false) {}
  Container(Container const& other) = delete;
  Container(Container&& other) {
    std::swap(m_con, other.m_con);
    std::swap(m_delete, other.m_delete);
  }
  Container(T* con, bool delete_needed) : m_con{con}, m_delete{delete_needed} {}
  virtual ~Container() {
    if (m_delete) {
      delete m_con;
    }
  }
  Container& operator=(Container const& other) = delete;
  Container& operator=(Container&& other) {
    std::swap(m_con, other.m_con);
    std::swap(m_delete, other.m_delete);
    return *this;
  }
  T& operator*() { return *m_con; }

 private:
  T* m_con;
  bool m_delete;
};

