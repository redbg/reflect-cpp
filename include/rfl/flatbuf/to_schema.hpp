#ifndef RFL_FLATBUF_TOSCHEMA_HPP_
#define RFL_FLATBUF_TOSCHEMA_HPP_

#include <map>
#include <string>
#include <type_traits>

#include "../Literal.hpp"
#include "../Processors.hpp"
#include "../Result.hpp"
#include "../SnakeCaseToCamelCase.hpp"
#include "../Variant.hpp"
#include "../json.hpp"
#include "../parsing/schema/Type.hpp"
#include "../parsing/schema/ValidationType.hpp"
#include "../parsing/schema/make.hpp"
#include "Reader.hpp"
#include "Schema.hpp"
#include "Writer.hpp"
#include "schema/Type.hpp"

namespace rfl::flatbuf {

/// This ensures that the schema is only generated once.
template <class T, class... Ps>
struct SchemaHolder {
  static SchemaHolder<T, Ps...> make() noexcept {
    const auto internal_schema =
        parsing::schema::make<Reader, Writer, T, Processors<Ps...>>();
    return SchemaHolder<T, Ps...>{Schema<T>(internal_schema)};
  }

  Schema<T> schema_;
};

template <class T, class... Ps>
static const SchemaHolder<T, Ps...> schema_holder =
    SchemaHolder<T, Ps...>::make();

/// Returns the flatbuffers schema for a class.
template <class T, class... Ps>
Schema<T> to_schema() {
  return schema_holder<T, Ps...>.schema_;
}
}  // namespace rfl::flatbuf

#endif
