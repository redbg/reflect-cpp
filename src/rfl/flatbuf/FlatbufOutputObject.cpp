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

#include "rfl/flatbuf/FlatbufOutputObject.hpp"

#include <type_traits>

#include "rfl/flatbuf/add_to_table.hpp"

namespace rfl::flatbuf {

FlatbufOutputObject::FlatbufOutputObject(const schema::Type::Table& _schema,
                                         FlatbufOutputParent* _parent,
                                         flatbuffers::FlatBufferBuilder* _fbb)
    : schema_(_schema), parent_(_parent), fbb_(_fbb) {}

void FlatbufOutputObject::add_offset(const flatbuffers::uoffset_t _offset) {
  static_assert(sizeof(flatbuffers::uoffset_t) <= sizeof(uint64_t),
                "Size cannot be greater than 4.");
  data_.push_back(0);
  std::memcpy(&data_.back(), &_offset, sizeof(flatbuffers::uoffset_t));
}

void FlatbufOutputObject::end() {
  if (data_.size() != schema_.fields.size()) {
    throw std::runtime_error(
        "data_.size(): " + std::to_string(data_.size()) +
        ", schema_.fields.size(): " + std::to_string(schema_.fields.size()));
  }
  const auto start = fbb_->StartTable();
  for (size_t i = 0; i < data_.size(); ++i) {
    add_to_table(i, schema_.fields[i].second, data_[i], fbb_);
  }
  auto offset = fbb_->EndTable(start);
  if (parent_) {
    parent_->add_offset(offset);
  } else {
    fbb_->Finish(flatbuffers::Offset<>(offset));
  }
}

}  // namespace rfl::flatbuf
