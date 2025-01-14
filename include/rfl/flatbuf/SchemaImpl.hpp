#ifndef RFL_FLATBUF_SCHEMAIMPL_HPP_
#define RFL_FLATBUF_SCHEMAIMPL_HPP_

#include <memory>
#include <string>

#include "../Result.hpp"
#include "flatbuf/schema/Type.hpp"

namespace rfl::flatbuf {

class SchemaImpl {
 public:
  SchemaImpl(const parsing::schema::Type& _internal_schema);

  ~SchemaImpl() = default;

  /// The string used to reproduce this schema.
  const std::string& str() const { return str_; }

  /// The interface used to create new values.
  const schema::Type& value() const { return *schema_; };

 private:
  /// The string used to create the schema.
  std::string str_;

  /// The actual schema
  Box<schema::Type> schema_;
};

}  // namespace rfl::flatbuf

#endif
