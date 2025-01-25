#include "rfl/flatbuf/FlatbufOutputArray.hpp"

#include <type_traits>

namespace rfl::flatbuf {

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

        } else if constexpr (std::is_same<U, schema::Type::Map>()) {
          /// The type of the offset does not matter...it is not used in any way
          /// by CreateVector.
          return do_create(TypeWrapper<flatbuffers::Offset<>>{});

        } else if constexpr (std::is_same<U, schema::Type::Optional>()) {
          throw std::runtime_error("TODO");  // TODO

        } else if constexpr (std::is_same<U, schema::Type::Vector>()) {
          /// The type of the offset does not matter...it is not used in any way
          /// by CreateVector.
          return do_create(TypeWrapper<flatbuffers::Offset<>>{});

        } else if constexpr (std::is_same<U, schema::Type::Reference>()) {
          if (!_t.type_ptr) {
            throw std::runtime_error("type_ptr not set for '" + _t.type_name +
                                     "'.");
          }
          build_vector(*_t.type_ptr);

        } else if constexpr (std::is_same<U, schema::Type::Table>()) {
          /// The type of the offset does not matter...it is not used in any way
          /// by CreateVector.
          return do_create(TypeWrapper<flatbuffers::Offset<>>{});

        } else if constexpr (std::is_same<U, schema::Type::Union>()) {
          throw std::runtime_error("TODO");  // TODO

        } else {
          static_assert(always_false_v<T>, "Unsupported type");
        }
      });
}

}  // namespace rfl::flatbuf
