#include <gtest/gtest.h>

#include <iostream>
#include <rfl.hpp>
#include <rfl/flatbuf.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_tutorial_example {

using Age = rfl::Validator<unsigned int, rfl::Minimum<0>, rfl::Maximum<130>>;

struct Person {
  std::string first_name;
  std::string last_name = "Simpson";
  Age age;
  std::vector<Person> children;
};

TEST(flatbuffers, test_person) {
  const auto bart = Person{.first_name = "Bart", .age = 10};
  const auto lisa = Person{.first_name = "Lisa", .age = 8};
  const auto maggie = Person{.first_name = "Maggie", .age = 0};
  const auto homer =
      Person{.first_name = "Homer",
             .age = 45,
             .children = std::vector<Person>({bart, lisa, maggie})};

  write_and_read(homer);
}
}  // namespace test_tutorial_example
