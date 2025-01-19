#ifndef RFL_FLATBUF_CALCVTABLEOFFSET_HPP_
#define RFL_FLATBUF_CALCVTABLEOFFSET_HPP_

#include <flatbuffers/flatbuffers.h>

namespace rfl::flatbuf {

inline flatbuffers::voffset_t calc_vtable_offset(const size_t _i) {
  return 2 * sizeof(flatbuffers::voffset_t) +
         static_cast<flatbuffers::voffset_t>(_i) *
             sizeof(flatbuffers::voffset_t);
}

}  // namespace rfl::flatbuf

#endif

