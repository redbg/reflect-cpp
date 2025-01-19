#ifndef RFL_FLATBUF_SCHEMA_TYPE_HPP_
#define RFL_FLATBUF_SCHEMA_TYPE_HPP_

#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "../../Literal.hpp"
#include "../../Object.hpp"
#include "../../Rename.hpp"
#include "../../Variant.hpp"

namespace rfl::flatbuf::schema {

struct Type {
  struct Bool {};

  struct Byte {};

  struct UByte {};

  struct Int8 {};

  struct Int16 {};

  struct Int32 {};

  struct Int64 {};

  struct UInt8 {};

  struct UInt16 {};

  struct UInt32 {};

  struct UInt64 {};

  struct Float32 {};

  struct Float64 {};

  struct String {};

  struct Enum {
    std::string name;
    std::vector<std::string> fields;
  };

  struct Optional {
    rfl::Ref<Type> type;
  };

  struct Vector {
    rfl::Ref<Type> type;
  };

  struct Map {
    rfl::Ref<Type> type;
  };

  struct Reference {
    std::string type_name;
  };

  struct Table {
    std::string name;
    std::vector<std::pair<std::string, Type>> fields;
  };

  struct Union {
    std::string name;
    std::vector<std::pair<std::string, Type>> fields;
  };

  using ReflectionType =
      rfl::Variant<Bool, Byte, UByte, Int8, Int16, Int32, Int64, UInt8, UInt16,
                   UInt32, UInt64, Float32, Float64, String, Enum, Optional,
                   Vector, Map, Reference, Table, Union>;

  const auto& reflection() const { return value; }

  Type with_name(const std::string& _name) const;

  ReflectionType value;
};

std::ostream& operator<<(std::ostream& os, const Type& _t);

}  // namespace rfl::flatbuf::schema
#endif
