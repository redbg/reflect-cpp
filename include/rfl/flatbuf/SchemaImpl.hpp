#ifndef RFL_FLATBUF_SCHEMAIMPL_HPP_
#define RFL_FLATBUF_SCHEMAIMPL_HPP_

#include <memory>
#include <string>

#include "../Result.hpp"
#include "../schema/Type.hpp"
#include "schema/FlatbufTypes.hpp"

namespace rfl::flatbuf {

class SchemaImpl {
 public:
  SchemaImpl(const parsing::schema::Type& _internal_schema);

  ~SchemaImpl() = default;

  /// The string used to reproduce this schema.
  std::string str() const;

  /// The interface used to create new values.
  const schema::Type& value() const { return schema_; };

 private:
  /// The flatbuffers schema.
  schema::FlatbufTypes schema_;
};

}  // namespace rfl::flatbuf

#endif
