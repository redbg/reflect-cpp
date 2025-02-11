/*

MIT License

Copyright (c) 2023-2024 Code17 GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "rfl/flatbuf/FlatbufOutputUnion.hpp"

#include <type_traits>

#include "rfl/flatbuf/calc_vtable_offset.hpp"

namespace rfl::flatbuf {

FlatbufOutputUnion::FlatbufOutputUnion(const schema::Type::Union& _schema,
                                       FlatbufOutputParent* _parent,
                                       flatbuffers::FlatBufferBuilder* _fbb)
    : schema_(_schema), parent_(_parent), fbb_(_fbb), index_(0), data_(0) {}

void FlatbufOutputUnion::add_offset(const flatbuffers::uoffset_t _offset) {
  static_assert(sizeof(flatbuffers::uoffset_t) <= sizeof(uint64_t),
                "Size cannot be greater than 4.");
  std::memcpy(&data_, &_offset, sizeof(flatbuffers::uoffset_t));
}

void FlatbufOutputUnion::end() {
  const auto inner_start = fbb_->StartTable();
  add_to_table(0, schema_.fields.at(index_).second, data_, fbb_);
  const auto inner_offset = fbb_->EndTable(inner_start);

  const auto outer_start = fbb_->StartTable();
  fbb_->AddElement<uint8_t>(calc_vtable_offset(0),
                            static_cast<uint8_t>(index_));
  fbb_->AddOffset<>(calc_vtable_offset(1), flatbuffers::Offset<>(inner_offset));
  const auto outer_offset = fbb_->EndTable(outer_start);

  if (parent_) {
    parent_->add_offset(outer_offset);
  } else {
    fbb_->Finish(flatbuffers::Offset<>(outer_offset));
  }
}

}  // namespace rfl::flatbuf
