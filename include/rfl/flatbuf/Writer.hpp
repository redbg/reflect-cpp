#ifndef RFL_FLATBUF_WRITER_HPP_
#define RFL_FLATBUF_WRITER_HPP_

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/flatbuffers.h>

#include <bit>
#include <cstdint>
#include <exception>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

#include "../Box.hpp"
#include "../Bytestring.hpp"
#include "../Ref.hpp"
#include "../Result.hpp"
#include "../always_false.hpp"
#include "../internal/is_literal.hpp"
#include "../internal/ptr_cast.hpp"

namespace rfl::flatbuf {

class Writer {
 public:
  struct FlatbufOutputArray {
    flatbuffers::uoffset_t offset_ = 0;
    size_t ix_ = 0;
  };

  struct FlatbufOutputMap {
    flatbuffers::uoffset_t offset_ = 0;
    size_t ix_ = 0;
  };

  struct FlatbufOutputObject {
    flatbuffers::voffset_t* field_offsets_ = nullptr;
    flatbuffers::uoffset_t offset_ = 0;
    size_t ix_ = 0;
  };

  struct FlatbufOutputUnion {
    flatbuffers::uoffset_t offset_ = 0;
  };

  struct FlatbufOutputVar {};

  using OutputArrayType = FlatbufOutputArray;
  using OutputMapType = FlatbufOutputMap;
  using OutputObjectType = FlatbufOutputObject;
  using OutputUnionType = FlatbufOutputUnion;
  using OutputVarType = FlatbufOutputVar;

  Writer(const Ref<flatbuffers::FlatBufferBuilder>& _fbb);

  ~Writer();

  template <class T>
  OutputArrayType array_as_root(const T _size) const noexcept {
    static_assert(always_false_v<T>,
                  "In flatbuffers, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T>
  OutputMapType map_as_root(const T _size) const noexcept {
    static_assert(always_false_v<T>,
                  "In flatbuffers, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  Writer::OutputObjectType object_as_root(const size_t _size) const noexcept;

  template <class T = int>
  OutputVarType null_as_root() const noexcept {
    static_assert(always_false_v<T>,
                  "In flatbuffers, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T = int>
  OutputUnionType union_as_root() const noexcept {
    static_assert(always_false_v<T>,
                  "In flatbuffers, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  template <class T>
  OutputVarType value_as_root(const T& _var) const noexcept {
    static_assert(always_false_v<T>,
                  "In flatbuffers, root values must always be structs.");
    throw std::runtime_error("Unsupported.");
  }

  OutputArrayType add_array_to_array(const size_t _size,
                                     OutputArrayType* _parent) const noexcept;

  OutputArrayType add_array_to_map(const std::string_view& _name,
                                   const size_t _size,
                                   OutputMapType* _parent) const noexcept;

  OutputArrayType add_array_to_object(const std::string_view& _name,
                                      const size_t _size,
                                      OutputObjectType* _parent) const noexcept;

  OutputArrayType add_array_to_union(const size_t _index, const size_t _size,
                                     OutputUnionType* _parent) const noexcept;

  OutputMapType add_map_to_array(const size_t _size,
                                 OutputArrayType* _parent) const noexcept;

  OutputMapType add_map_to_map(const std::string_view& _name,
                               const size_t _size,
                               OutputMapType* _parent) const noexcept;

  OutputMapType add_map_to_object(const std::string_view& _name,
                                  const size_t _size,
                                  OutputObjectType* _parent) const noexcept;

  OutputMapType add_map_to_union(const size_t _index, const size_t _size,
                                 OutputUnionType* _parent) const noexcept;

  OutputObjectType add_object_to_array(const size_t _size,
                                       OutputArrayType* _parent) const noexcept;

  OutputObjectType add_object_to_map(const std::string_view& _name,
                                     const size_t _size,
                                     OutputMapType* _parent) const noexcept;

  OutputObjectType add_object_to_object(
      const std::string_view& _name, const size_t _size,
      OutputObjectType* _parent) const noexcept;

  OutputObjectType add_object_to_union(const size_t _index, const size_t _size,
                                       OutputUnionType* _parent) const noexcept;

  OutputUnionType add_union_to_array(OutputArrayType* _parent) const noexcept;

  OutputUnionType add_union_to_map(const std::string_view& _name,
                                   OutputMapType* _parent) const noexcept;

  OutputUnionType add_union_to_object(const std::string_view& _name,
                                      OutputObjectType* _parent) const noexcept;

  OutputUnionType add_union_to_union(const size_t _index,
                                     OutputUnionType* _parent) const noexcept;

  OutputVarType add_null_to_array(OutputArrayType* _parent) const noexcept;

  OutputVarType add_null_to_map(const std::string_view& _name,
                                OutputMapType* _parent) const noexcept;

  OutputVarType add_null_to_object(const std::string_view& _name,
                                   OutputObjectType* _parent) const noexcept;

  OutputVarType add_null_to_union(const size_t _index,
                                  OutputUnionType* _parent) const noexcept;

  template <class T>
  OutputVarType add_value_to_array(const T& _var,
                                   OutputArrayType* _parent) const noexcept {
    // TODO
  }

  template <class T>
  OutputVarType add_value_to_map(const std::string_view& _name, const T& _var,
                                 OutputMapType* _parent) const noexcept {
    // TODO
  }

  template <class T>
  OutputVarType add_value_to_object(const std::string_view& _name,
                                    const T& _var,
                                    OutputObjectType* _parent) const noexcept {
    if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
      const auto str = fbb_->CreateString(_var.c_str(), _var.size());
      fbb_->AddOffset(_parent->field_offsets_[_parent->ix_++], str);

      // TODO
      // } else if constexpr (std::is_same<std::remove_cvref_t<T>,
      //                                   rfl::Bytestring>()) {

    } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                         std::is_same<std::remove_cvref_t<T>, bool>() ||
                         std::is_integral<std::remove_cvref_t<T>>()) {
      fbb_->AddElement<T>(_parent->field_offsets_[_parent->ix_++], _var);

      // TODO
      //} else if constexpr (internal::is_literal_v<T>) {
      //  return add_value_to_object(_name, _var.value(), _parent);

    } else {
      static_assert(rfl::always_false_v<T>, "Unsupported type.");
    }
    return OutputVarType{};
  }

  template <class T>
  OutputVarType add_value_to_union(const size_t _index, const T& _var,
                                   OutputUnionType* _parent) const noexcept {
    // TODO
    /*const auto field = _parent->val_.getSchema().getFields()[_index];

  if constexpr (std::is_same<std::remove_cvref_t<T>, std::string>()) {
    _parent->val_.set(field, _var.c_str());

  } else if constexpr (std::is_same<std::remove_cvref_t<T>,
                                    rfl::Bytestring>()) {
    const auto array_ptr = kj::ArrayPtr<const kj::byte>(
        internal::ptr_cast<const unsigned char*>(_var.data()), _var.size());
    _parent->val_.set(field, capnp::Data::Reader(array_ptr));

  } else if constexpr (std::is_floating_point<std::remove_cvref_t<T>>() ||
                       std::is_same<std::remove_cvref_t<T>, bool>()) {
    _parent->val_.set(field, _var);

  } else if constexpr (std::is_integral<std::remove_cvref_t<T>>()) {
    _parent->val_.set(field, static_cast<std::int64_t>(_var));

  } else if constexpr (internal::is_literal_v<T>) {
    return add_value_to_union(_index, _var.value(), _parent);

  } else {
    static_assert(rfl::always_false_v<T>, "Unsupported type.");
  }*/
    return OutputVarType{};
  }

  void end_array(OutputArrayType* _arr) const noexcept {}

  void end_map(OutputMapType* _obj) const noexcept {}

  void end_object(OutputObjectType* _obj) const noexcept {}

 private:
  Ref<flatbuffers::FlatBufferBuilder> fbb_;
};

}  // namespace rfl::flatbuf

#endif
