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

std::ostream& operator<<(std::ostream& _os,
                         const std::map<std::string, Type>& _map) {
  for (const auto& [name, type] : _map) {
    _os << type.with_name(name) << std::endl << std::endl;
  }
  return _os;
}

std::ostream& operator<<(std::ostream& _os,
                         const FlatbufTypes& _flatbuf_types) {
  _os << _flatbuf_types.structs_ << _flatbuf_types.enums_
      << _flatbuf_types.tuples_ << _flatbuf_types.unions_;
  return _os;
}

}  // namespace rfl::flatbuf::schema

#endif
