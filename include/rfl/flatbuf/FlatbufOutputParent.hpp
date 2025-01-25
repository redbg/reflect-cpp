
#ifndef RFL_FLATBUF_FLATBUFOUTPUTPARENT_HPP_
#define RFL_FLATBUF_FLATBUFOUTPUTPARENT_HPP_

#include <flatbuffers/flatbuffers.h>

namespace rfl::flatbuf {
struct FlatbufOutputParent {
  virtual void add_offset(const flatbuffers::uoffset_t _offset) = 0;
};

}  // namespace rfl::flatbuf

#endif
