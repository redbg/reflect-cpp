#ifndef RFL_FLATBUF_SCHEMA_HPP_
#define RFL_FLATBUF_SCHEMA_HPP_

#include <capnp/schema-parser.h>

#include <type_traits>

#include "../Ref.hpp"
#include "SchemaImpl.hpp"

namespace rfl::flatbuf {

template <class T>
class Schema {
 public:
  using Type = std::remove_cvref_t<T>;

  Schema(const parsing::schema::Type& _internal_schema)
      : impl_(Ref<SchemaImpl>::make(_internal_schema)) {}

  /// The string used to create this schema.
  std::string str() const { return impl_->str(); }

  /// The struct schema used to generate new values.
  const schema::Type& value() const { return impl_->value(); };

 private:
  /// We are using the "pimpl"-pattern
  Ref<SchemaImpl> impl_;
};

}  // namespace rfl::flatbuf

#endif
