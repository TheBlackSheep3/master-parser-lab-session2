#pragma once

#include <utility>

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

