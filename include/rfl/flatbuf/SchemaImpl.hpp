#ifndef RFL_FLATBUF_SCHEMAIMPL_HPP_
#define RFL_FLATBUF_SCHEMAIMPL_HPP_

#include <memory>
#include <string>

#include "../Result.hpp"
#include "../parsing/schema/Definition.hpp"
#include "schema/FlatbufSchema.hpp"
#include "schema/Type.hpp"

namespace rfl::flatbuf {

class SchemaImpl {
 public:
  SchemaImpl(const parsing::schema::Definition& _internal_schema);

  ~SchemaImpl() = default;

  /// The string used to reproduce this schema.
  std::string str() const;

  /// The interface used to create new values.
  const schema::FlatbufSchema& value() const { return schema_.value(); };

 private:
  /// The flatbuffers schema.
  Result<schema::FlatbufSchema> schema_;
};

}  // namespace rfl::flatbuf

#endif
