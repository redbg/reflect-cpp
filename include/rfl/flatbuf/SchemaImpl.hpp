#ifndef RFL_FLATBUF_SCHEMAIMPL_HPP_
#define RFL_FLATBUF_SCHEMAIMPL_HPP_

#include <memory>
#include <string>

#include "../Result.hpp"
#include "../parsing/schema/Definition.hpp"
#include "schema/FlatbufTypes.hpp"
#include "schema/Type.hpp"

namespace rfl::flatbuf {

class SchemaImpl {
 public:
  SchemaImpl(const parsing::schema::Definition& _internal_schema);

  ~SchemaImpl() = default;

  /// The string used to reproduce this schema.
  std::string str() const;

  /// The interface used to create new values.
  const schema::FlatbufTypes& value() const { return schema_; };

 private:
  /// The flatbuffers schema.
  schema::FlatbufTypes schema_;
};

}  // namespace rfl::flatbuf

#endif
