#ifndef RFL_FLATBUF_SCHEMA_INTERNAL_SCHEMA_TO_FLATBUF_SCHEMA_HPP_
#define RFL_FLATBUF_SCHEMA_INTERNAL_SCHEMA_TO_FLATBUF_SCHEMA_HPP_

#include "../../parsing/schema/Type.hpp"
#include "FlatbufTypes.hpp"
#include "Type.hpp"

namespace rfl::flatbuf::schema {

/// Transforms an internal schema to a flatbuffers schema. Used by
/// SchemaImpl.
FlatbufTypes internal_schema_to_flatbuf_schema(
    const parsing::schema::Type& _type);

}  // namespace rfl::flatbuf::schema

#endif  // RFL_FLATBUF_SCHEMA_INTERNAL_SCHEMA_TO_FLATBUF_SCHEMA_HPP_
