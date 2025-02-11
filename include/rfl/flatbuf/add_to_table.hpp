#ifndef RFL_FLATBUF_ADDTOTABLE_HPP_
#define RFL_FLATBUF_ADDTOTABLE_HPP_

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/flatbuffers.h>

#include <cstdint>

namespace rfl::flatbuf {

void add_to_table(const size_t _i, const schema::Type& _type,
                  const uint64_t _val, flatbuffers::FlatBufferBuilder* _fbb);

}  // namespace rfl::flatbuf

#endif

