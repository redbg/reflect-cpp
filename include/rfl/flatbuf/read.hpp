#ifndef RFL_FLATBUF_READ_HPP_
#define RFL_FLATBUF_READ_HPP_

#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/verifier.h>

#include <bit>
#include <istream>
#include <string>
#include <type_traits>

#include "../Processors.hpp"
#include "../Ref.hpp"
#include "../internal/strings/strings.hpp"
#include "../internal/wrap_in_rfl_array_t.hpp"
#include "Parser.hpp"
#include "Reader.hpp"

namespace rfl::flatbuf {

using InputObjectType = typename Reader::InputObjectType;
using InputVarType = typename Reader::InputVarType;

/// Parses an object from flatbuffers using reflection.
template <class T, class... Ps>
Result<internal::wrap_in_rfl_array_t<T>> read(const char* _bytes,
                                              const size_t _size) {
  const auto input_var = InputVarType{flatbuffers::GetRoot<const uint8_t>(
      internal::ptr_cast<const uint8_t*>(_bytes))};
  const auto verifier = Ref<flatbuffers::Verifier>::make(
      flatbuffers::Verifier(internal::ptr_cast<const uint8_t*>(_bytes), _size,
                            flatbuffers::Verifier::Options{}));
  const auto r = Reader(verifier);
  return Parser<T, Processors<Ps...>>::read(r, input_var);
}

/// Parses an object from flatbuffers using reflection.
template <class T, class... Ps>
auto read(const std::vector<char>& _bytes) {
  return read<T, Ps...>(_bytes.data(), _bytes.size());
}

/// Parses an object from a stream.
template <class T, class... Ps>
auto read(std::istream& _stream) {
  std::istreambuf_iterator<char> begin(_stream), end;
  auto bytes = std::vector<char>(begin, end);
  return read<T, Ps...>(bytes.data(), bytes.size());
}

}  // namespace rfl::flatbuf

#endif
