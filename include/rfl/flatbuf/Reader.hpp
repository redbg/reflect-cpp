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
#include "rfl/parsing/schemaful/IsSchemafulReader.hpp"

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
    const flatbuffers::Table* val_;
  };

  struct FlatbufInputVar {
    const uint8_t* val_;
  };

  using InputArrayType = FlatbufInputArray;
  using InputObjectType = FlatbufInputObject;
  using InputMapType = FlatbufInputMap;
  using InputUnionType = FlatbufInputUnion;
  using InputVarType = FlatbufInputVar;

  Reader(const uint8_t* _root, const Ref<flatbuffers::Verifier>& _verifier)
      : root_(_root), verifier_(_verifier) {}

  template <class T>
  static constexpr bool has_custom_constructor =
      (requires(InputVarType var) { T::from_flatbuf_obj(var); });

  bool is_empty(const InputVarType& _var) const noexcept { return !_var.val_; }

  template <class T>
  rfl::Result<T> to_basic_type(const InputVarType& _var) const noexcept {
    if (!_var.val_) {
      return error("Could not cast, was a nullptr.");
    }
    using Type = std::remove_cvref_t<T>;
    if constexpr (std::is_same<Type, std::string>()) {
      const auto* str = internal::ptr_cast<const flatbuffers::String*>(
          apply_ptr_correction(_var.val_));
      if (!verifier_->VerifyString(str)) {
        return error("Could not cast to string.");
      };
      return str->str();

      /*} else if constexpr (std::is_same<Type,
                                        rfl::Bytestring>()) {
        if (type != capnp::DynamicValue::DATA) {
          return Error("Could not cast to bytestring.");
        }
        const auto data = _var.val_.as<capnp::Data>();
        return rfl::Bytestring(internal::ptr_cast<const
        std::byte*>(data.begin()), data.size());*/

    } else if constexpr (std::is_same<Type, bool>() ||
                         std::is_floating_point<Type>() ||
                         std::is_integral<Type>()) {
      if (!verifier_->VerifyField<Type>(_var.val_, 0, alignof(Type))) {
        return error("Could not verify.");
      }
      return flatbuffers::ReadScalar<Type>(_var.val_);

      /*} else if constexpr (internal::is_literal_v<T>) {
        if (type != capnp::DynamicValue::ENUM) {
          return rfl::Error("Could not cast to an enum.");
        }
        return T::from_value(_var.val_.as<capnp::DynamicEnum>().getRaw());*/

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
  }

  rfl::Result<InputArrayType> to_array(
      const InputVarType& _var) const noexcept {
    if (!_var.val_) {
      return error("Could not cast to a vector: Is null.");
    }
    const auto ptr = internal::ptr_cast<const flatbuffers::VectorOfAny*>(
        apply_ptr_correction(_var.val_));
    return InputArrayType{ptr};
  }

  rfl::Result<InputObjectType> to_object(
      const InputVarType& _var) const noexcept {
    if (!_var.val_) {
      return error("Could not cast to a table: Is null.");
    }
    const auto ptr = internal::ptr_cast<const flatbuffers::Table*>(
        apply_ptr_correction(_var.val_));
    return InputObjectType{ptr};
  }

  rfl::Result<InputMapType> to_map(const InputVarType& _var) const noexcept {
    // TODO
  }

  rfl::Result<InputUnionType> to_union(
      const InputVarType& _var) const noexcept {
    if (!_var.val_) {
      return error("Could not cast to a table: Is null.");
    }
    const auto ptr = internal::ptr_cast<const flatbuffers::Table*>(
        apply_ptr_correction(_var.val_));
    return InputUnionType{ptr};
  }

  template <class ArrayReader>
  std::optional<Error> read_array(const ArrayReader& _array_reader,
                                  const InputArrayType& _arr) const noexcept {
    constexpr size_t elem_size =
        calc_elem_size<typename ArrayReader::ValueType>();
    if (!verifier_->VerifyVectorOrString(
            internal::ptr_cast<const uint8_t*>(_arr.val_), elem_size)) {
      return Error("Verification of the vector failed.");
    }
    for (size_t i = 0; i < _arr.val_->size(); ++i) {
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
      _object_reader.read(i, InputVarType{_obj.val_->GetAddressOf(offset)});
    }
    verifier_->EndTable();
    return std::nullopt;
  }

  template <class VariantType, class UnionReaderType>
  rfl::Result<VariantType> read_union(
      const InputUnionType& _union) const noexcept {
    if (!_union.val_->VerifyTableStart(*verifier_)) {
      return error("Table start could not be verified.");
    }

    const auto disc_val = _union.val_->GetAddressOf(calc_vtable_offset(0));
    if (!verifier_->VerifyField<uint8_t>(disc_val, 0, alignof(uint8_t))) {
      return error("Could not verify the type of the discriminant.");
    }
    const auto disc = flatbuffers::ReadScalar<uint8_t>(disc_val);

    const auto retrieve_value = [&](const auto& _inner_table) {
      if (!_inner_table.val_->VerifyTableStart(*verifier_)) {
        return error("Table start could not be verified.");
      }
      const auto val =
          InputVarType{_inner_table->GetAddressOf(calc_vtable_offset(0))};
      verifier_->EndTable();
      return UnionReaderType::read(*this, static_cast<size_t>(disc) - 1, val);
    };

    const auto result =
        disc == 0 ? UnionReaderType::read(*this, 0, InputVarType{nullptr})
                  : to_object(InputVarType{_union.val_->GetAddressOf(
                                  calc_vtable_offset(1))})
                        .and_then(retrieve_value);

    verifier_->EndTable();

    return result;
  }

  template <class T>
  rfl::Result<T> use_custom_constructor(
      const InputVarType& _var) const noexcept {
    try {
      return T::from_flatbuf_obj(_var);
    } catch (std::exception& e) {
      return error(e.what());
    }
  }

 private:
  const uint8_t* apply_ptr_correction(const uint8_t* _ptr) const noexcept {
    if (_ptr == root_) {
      return _ptr;
    }
    return _ptr + flatbuffers::ReadScalar<flatbuffers::uoffset_t>(_ptr);
  }

 private:
  /// The root node.
  const uint8_t* root_;

  /// Used to verify the input stream.
  Ref<flatbuffers::Verifier> verifier_;
};

static_assert(parsing::schemaful::IsSchemafulReader<Reader>,
              "This must be a schemaful reader.");

}  // namespace rfl::flatbuf

#endif
