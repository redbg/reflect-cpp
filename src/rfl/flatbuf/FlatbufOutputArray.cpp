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

#include "rfl/flatbuf/FlatbufOutputArray.hpp"

#include <type_traits>

namespace rfl::flatbuf {

void FlatbufOutputArray::add_offset(const flatbuffers::uoffset_t _offset) {
  auto offset = flatbuffers::Offset<>(_offset);
  const auto ptr = internal::ptr_cast<const uint8_t*>(&offset);
  data_.insert(data_.end(), ptr, ptr + sizeof(flatbuffers::Offset<>));
}

flatbuffers::uoffset_t FlatbufOutputArray::build_vector(
    const schema::Type& _type) const {
  const auto do_create =
      [&]<class T>(const TypeWrapper<T>&) -> flatbuffers::uoffset_t {
    if constexpr (std::is_same<T, flatbuffers::Offset<>>()) {
      return fbb_
          ->CreateVector<void>(
              internal::ptr_cast<const flatbuffers::Offset<>*>(data_.data()),
              data_.size() / sizeof(flatbuffers::Offset<>))
          .o;
    } else {
      return fbb_
          ->CreateVector<T>(internal::ptr_cast<const T*>(data_.data()),
                            data_.size() / sizeof(T))
          .o;
    }
  };

  return _type.reflection().visit(
      [&]<class T>(const T& _t) -> flatbuffers::uoffset_t {
        using U = std::remove_cvref_t<T>;
        if constexpr (std::is_same<U, schema::Type::Bool>()) {
          throw std::runtime_error("TODO");  // TODO

        } else if constexpr (std::is_same<U, schema::Type::Byte>()) {
          throw std::runtime_error("TODO");  // TODO

        } else if constexpr (std::is_same<U, schema::Type::UByte>()) {
          throw std::runtime_error("TODO");  // TODO

        } else if constexpr (std::is_same<U, schema::Type::Int8>()) {
          return do_create(TypeWrapper<int8_t>{});

        } else if constexpr (std::is_same<U, schema::Type::Int16>()) {
          return do_create(TypeWrapper<int16_t>{});

        } else if constexpr (std::is_same<U, schema::Type::Int32>()) {
          return do_create(TypeWrapper<int32_t>{});

        } else if constexpr (std::is_same<U, schema::Type::Int64>()) {
          return do_create(TypeWrapper<int64_t>{});

        } else if constexpr (std::is_same<U, schema::Type::UInt8>()) {
          return do_create(TypeWrapper<uint8_t>{});

        } else if constexpr (std::is_same<U, schema::Type::UInt16>()) {
          return do_create(TypeWrapper<uint16_t>{});

        } else if constexpr (std::is_same<U, schema::Type::UInt32>()) {
          return do_create(TypeWrapper<uint32_t>{});

        } else if constexpr (std::is_same<U, schema::Type::UInt64>()) {
          return do_create(TypeWrapper<uint64_t>{});

        } else if constexpr (std::is_same<U, schema::Type::Float32>()) {
          return do_create(TypeWrapper<float>{});

        } else if constexpr (std::is_same<U, schema::Type::Float64>()) {
          return do_create(TypeWrapper<double>{});

        } else if constexpr (std::is_same<U, schema::Type::String>()) {
          return do_create(TypeWrapper<flatbuffers::Offset<>>{});

        } else if constexpr (std::is_same<U, schema::Type::Enum>()) {
          throw std::runtime_error("TODO");  // TODO

        } else if constexpr (std::is_same<U, schema::Type::Vector>()) {
          return do_create(TypeWrapper<flatbuffers::Offset<>>{});

        } else if constexpr (std::is_same<U, schema::Type::Reference>()) {
          if (!_t.type_ptr) {
            throw std::runtime_error("type_ptr not set for '" + _t.type_name +
                                     "'.");
          }
          return build_vector(*_t.type_ptr);

        } else if constexpr (std::is_same<U, schema::Type::Table>()) {
          return do_create(TypeWrapper<flatbuffers::Offset<>>{});

        } else if constexpr (std::is_same<U, schema::Type::Union>()) {
          throw std::runtime_error("TODO");  // TODO

        } else {
          static_assert(always_false_v<T>, "Unsupported type");
        }
      });
}

}  // namespace rfl::flatbuf
