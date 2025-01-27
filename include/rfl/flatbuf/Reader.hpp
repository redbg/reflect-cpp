#ifndef RFL_FLATBUF_READER_HPP_
#define RFL_FLATBUF_READER_HPP_

#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/reflection.h>
#include <flatbuffers/verifier.h>

#include <cstddef>
#include <exception>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../Bytestring.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/is_literal.hpp"
#include "../internal/ptr_cast.hpp"
#include "calc_elem_size.hpp"
#include "calc_vtable_offset.hpp"

namespace rfl::flatbuf {

class Reader {
 public:
  struct FlatbufInputArray {
    const flatbuffers::VectorOfAny* val_;
  };

  struct FlatbufInputMap {
    const uint8_t* val_;
  };

  struct FlatbufInputObject {
    const flatbuffers::Table* val_;
  };

  struct FlatbufInputUnion {
    const uint8_t* val_;
  };

  struct FlatbufInputVar {
    const uint8_t* val_;
  };

  using InputArrayType = FlatbufInputArray;
  using InputObjectType = FlatbufInputObject;
  using InputMapType = FlatbufInputMap;
  using InputUnionType = FlatbufInputUnion;
  using InputVarType = FlatbufInputVar;

  Reader(const Ref<flatbuffers::Verifier>& _verifier);

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_flatbuf_obj(var); });

  bool is_empty(const InputVarType& _var) const noexcept;

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    std::cout << "to_basic_type1" << std::endl;
    if (!_var.val_) {
      return Error("Could not cast, was a nullptr.");
    }
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      return internal::ptr_cast<const flatbuffers::String*>(
                 apply_ptr_correction(_var.val_))
          ->str();

      /*} else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                        rfl::Bytestring>()) {
        if (type != capnp::DynamicValue::DATA) {
          return Error("Could not cast to bytestring.");
        }
        const auto data = _var.val_.as<capnp::Data>();
        return rfl::Bytestring(internal::ptr_cast<const
        std::byte*>(data.begin()), data.size());*/

    } else if constexpr (std::is_same<std::remove_cvref_t<T>, bool>() ||
                         std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      return *flatbuffers::ReadScalar<const T*>(_var.val_);

      /*} else if constexpr (internal::is_literal_v<T>) {
        if (type != capnp::DynamicValue::ENUM) {
          return rfl::Error("Could not cast to an enum.");
        }
        return T::from_value(_var.val_.as<capnp::DynamicEnum>().getRaw());*/

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(const InputVarType& _var) const noexcept;

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept;

  rfl::Result<InputMapType> to_map(const InputVarType& _var) const noexcept;

  rfl::Result<InputUnionType> to_union(const InputVarType& _var) const noexcept;

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    constexpr size_t elem_size =
        calc_elem_size<typename ArrayReader::ValueType>();
    const size_t size = static_cast<size_t>(_arr.val_->size()) / elem_size;
    for (size_t i = 0; i < size; ++i) {
      const auto err = _array_reader.read(
          InputVarType{flatbuffers::GetAnyVectorElemAddressOf<const uint8_t>(
              _arr.val_, i, elem_size)});
      if (err) {
        return err;
      }
    }
    return std::nullopt;
  }

  template <class MapReader>
  std::optional<Error> read_map(const MapReader& _map_reader,
                                const InputMapType& _map) const noexcept {
    // TODO
  }

  template <class ObjectReader>
  std::optional<Error> read_object(const ObjectReader& _object_reader,
                                   const InputObjectType& _obj) const noexcept {
    if (!_obj.val_->VerifyTableStart(*verifier_)) {
      return Error("Table start could not be verified.");
    }
    constexpr size_t size = ObjectReader::size();
    for (size_t i = 0; i < size; ++i) {
      const auto offset = calc_vtable_offset(i);
      if (!_obj.val_->VerifyOffset(*verifier_, offset)) {
        return Error("Offset for field " + std::to_string(i + 1) +
                     " could not be verified.");
      }
      _object_reader.read(i, InputVarType{_obj.val_->GetAddressOf(offset)});
    }
    verifier_->EndTable();
    return std::nullopt;
  }

  template <class VariantType, class UnionReaderType>
  rfl::Result<VariantType> read_union(
      const InputUnionType& _union) const noexcept {
    // TODO
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_flatbuf_obj(_var);
    } catch (std::exception& e) {
      return rfl::Error(e.what());
    }
  }

 private:
  const uint8_t* apply_ptr_correction(const uint8_t* _ptr) const noexcept {
    return _ptr + flatbuffers::ReadScalar<flatbuffers::uoffset_t>(_ptr);
  }

 private:
  /// Used to verify the input stream.
  Ref<flatbuffers::Verifier> verifier_;
};

}  // namespace rfl::flatbuf

#endif
