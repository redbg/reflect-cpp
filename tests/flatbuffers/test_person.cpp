#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/flatbuf.hpp>
#include <string>
#include <vector>

// #include "write_and_read.hpp"

namespace test_tutorial_example {

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  std::vector<Person> children;
};

TEST(flatbuffers, test_person) {
  std::cout << rfl::flatbuf::to_schema<Person>().value() << std::endl;
}
}  // namespace test_tutorial_example
