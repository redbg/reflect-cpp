#ifndef RFL_FLATBUF_CALCELEMSIZE_HPP_
#define RFL_FLATBUF_CALCELEMSIZE_HPP_

#include <type_traits>

namespace rfl::flatbuf {

template <class T>
consteval size_t calc_elem_size() {
  using Type = std::remove_cvref_t<T>;
  if constexpr (std::is_same<Type, bool>() || std::is_floating_point<Type>() ||
                std::is_integral<Type>()) {
    return sizeof(Type);
  } else {
    return 4;
  }
}

}  // namespace rfl::flatbuf

#endif

