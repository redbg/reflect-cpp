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
    add_to_table(i, schema_.fields[i].second, data_[i]);
  }
  auto offset = fbb_->EndTable(start);
  if (parent_) {
    parent_->add_offset(offset);
  } else {
    fbb_->Finish(flatbuffers::Offset<>(offset));
  }
}

void FlatbufOutputObject::add_to_table(const size_t _i,
                                       const schema::Type& _type,
                                       const uint64_t _val) {
  const auto do_add = [&]<class T>(const TypeWrapper<T>&) {
    if constexpr (std::is_same<T, flatbuffers::Offset<>>()) {
      fbb_->AddOffset<>(
          calc_vtable_offset(_i),
          flatbuffers::Offset<>(
              *internal::ptr_cast<const flatbuffers::uoffset_t*>(&_val)));
    } else {
      fbb_->AddElement<T>(calc_vtable_offset(_i),
                          *internal::ptr_cast<const T*>(&_val));
    }
  };

  _type.reflection().visit([&]<class T>(const T& _t) {
    using U = std::remove_cvref_t<T>;
    if constexpr (std::is_same<U, schema::Type::Bool>()) {
      throw std::runtime_error("TODO");  // TODO

    } else if constexpr (std::is_same<U, schema::Type::Byte>()) {
      throw std::runtime_error("TODO");  // TODO

    } else if constexpr (std::is_same<U, schema::Type::UByte>()) {
      throw std::runtime_error("TODO");  // TODO

    } else if constexpr (std::is_same<U, schema::Type::Int8>()) {
      return do_add(TypeWrapper<int8_t>{});

    } else if constexpr (std::is_same<U, schema::Type::Int16>()) {
      return do_add(TypeWrapper<int16_t>{});

    } else if constexpr (std::is_same<U, schema::Type::Int32>()) {
      return do_add(TypeWrapper<int32_t>{});

    } else if constexpr (std::is_same<U, schema::Type::Int64>()) {
      return do_add(TypeWrapper<int64_t>{});

    } else if constexpr (std::is_same<U, schema::Type::UInt8>()) {
      return do_add(TypeWrapper<uint8_t>{});

    } else if constexpr (std::is_same<U, schema::Type::UInt16>()) {
      return do_add(TypeWrapper<uint16_t>{});

    } else if constexpr (std::is_same<U, schema::Type::UInt32>()) {
      return do_add(TypeWrapper<uint32_t>{});

    } else if constexpr (std::is_same<U, schema::Type::UInt64>()) {
      return do_add(TypeWrapper<uint64_t>{});

    } else if constexpr (std::is_same<U, schema::Type::Float32>()) {
      return do_add(TypeWrapper<float>{});

    } else if constexpr (std::is_same<U, schema::Type::Float64>()) {
      return do_add(TypeWrapper<double>{});

    } else if constexpr (std::is_same<U, schema::Type::String>() ||
                         std::is_same<U, schema::Type::Map>() ||
                         std::is_same<U, schema::Type::Vector>() ||
                         std::is_same<U, schema::Type::Table>()

    ) {
      return do_add(TypeWrapper<flatbuffers::Offset<>>{});

    } else if constexpr (std::is_same<U, schema::Type::Enum>()) {
      throw std::runtime_error("TODO");  // TODO

    } else if constexpr (std::is_same<U, schema::Type::Reference>()) {
      if (!_t.type_ptr) {
        throw std::runtime_error("type_ptr not set for '" + _t.type_name +
                                 "'.");
      }
      return add_to_table(_i, *_t.type_ptr, _val);

    } else if constexpr (std::is_same<U, schema::Type::Union>()) {
      throw std::runtime_error("TODO");  // TODO

    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
  });
}

}  // namespace rfl::flatbuf
