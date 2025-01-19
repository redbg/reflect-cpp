#ifndef RFL_FLATBUF_SCHEMA_FLATBUFTYPES_HPP_
#define RFL_FLATBUF_SCHEMA_FLATBUFTYPES_HPP_

#include <map>

#include "Type.hpp"

namespace rfl::flatbuf::schema {

struct FlatbufTypes {
  std::map<std::string, Type> structs_;
  std::map<std::string, Type> enums_;
  std::map<std::string, Type> tuples_;
  std::map<std::string, Type> unions_;
  Type root_;
};

std::ostream& operator<<(std::ostream& _os, const FlatbufTypes& _flatbuf_types);

}  // namespace rfl::flatbuf::schema

#endif
