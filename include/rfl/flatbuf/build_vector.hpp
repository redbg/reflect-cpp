#ifndef RFL_FLATBUF_BUILDVECTOR_HPP_
#define RFL_FLATBUF_BUILDVECTOR_HPP_

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/flatbuffers.h>

#include <cstdint>
#include <vector>

#include "schema/Type.hpp"

namespace rfl::flatbuf {

flatbuffers::uoffset_t build_vector(const schema::Type& _type,
                                    const std::vector<uint8_t>& _data,
                                    flatbuffers::FlatBufferBuilder* _fbb);

}  // namespace rfl::flatbuf

#endif

