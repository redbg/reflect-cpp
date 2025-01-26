#ifndef RFL_FLATBUF_WRITE_HPP_
#define RFL_FLATBUF_WRITE_HPP_

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/flatbuffers.h>

#include <bit>
#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "../SnakeCaseToCamelCase.hpp"
#include "../internal/ptr_cast.hpp"
#include "../internal/strings/strings.hpp"
#include "../parsing/Parent.hpp"
#include "Parser.hpp"
#include "Schema.hpp"
#include "Writer.hpp"
#include "to_schema.hpp"

namespace rfl::flatbuf {

/// Returns FLATBUF bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj, const auto& _schema) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  using U = typename std::remove_cvref_t<decltype(_schema)>::Type;
  using ParentType = parsing::Parent<Writer>;
  static_assert(std::is_same<T, U>(),
                "The schema must be compatible with the type to write.");
  const auto fbb = Ref<flatbuffers::FlatBufferBuilder>::make();
  const auto writer = Writer(fbb, _schema.value());
  Parser<T, Processors<Ps...>>::write(writer, _obj,
                                      typename ParentType::Root{});
  const auto ptr = fbb->GetBufferPointer();
  return std::vector<char>(internal::ptr_cast<char*>(ptr),
                           internal::ptr_cast<char*>(ptr + fbb->GetSize()));
}

/// Returns FLATBUF bytes.
template <class... Ps>
std::vector<char> write(const auto& _obj) {
  using T = std::remove_cvref_t<decltype(_obj)>;
  const auto schema = to_schema<T, Ps...>();
  return write<Ps...>(_obj, schema);
}

/// Writes a FLATBUF into an ostream.
template <class... Ps>
std::ostream& write(const auto& _obj, std::ostream& _stream) {
  auto buffer = write<Ps...>(_obj);
  _stream.write(buffer.data(), buffer.size());
  return _stream;
}

}  // namespace rfl::flatbuf

#endif
