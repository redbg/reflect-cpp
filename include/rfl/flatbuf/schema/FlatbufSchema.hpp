#ifndef RFL_FLATBUF_SCHEMA_FLATBUFSCHEMA_HPP_
#define RFL_FLATBUF_SCHEMA_FLATBUFSCHEMA_HPP_

#include <map>

#include "../../Ref.hpp"
#include "../../Result.hpp"
#include "Type.hpp"

namespace rfl::flatbuf::schema {

struct FlatbufSchema {
  Ref<std::map<std::string, schema::Type>> structs_;
  Ref<std::map<std::string, schema::Type>> enums_;
  Ref<std::map<std::string, schema::Type>> tuples_;
  Ref<std::map<std::string, schema::Type>> unions_;

  Type::Reference root_type_;

  Result<FlatbufSchema> set_reference_ptrs() const;
};

std::ostream& operator<<(std::ostream& _os,
                         const FlatbufSchema& _flatbuf_schema);

}  // namespace rfl::flatbuf::schema

#endif
