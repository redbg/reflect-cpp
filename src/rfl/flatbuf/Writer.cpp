#include "rfl/flatbuf/Writer.hpp"

#include <stdexcept>

#include "rfl/always_false.hpp"
#include "rfl/parsing/schemaful/IsSchemafulWriter.hpp"

namespace rfl::flatbuf {

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>,
              "This must be a schemaful writer.");

Writer::Writer(const Ref<flatbuffers::FlatBufferBuilder>& _fbb,
               const Ref<schema::FlatbufSchema>& _schema)
    : fbb_(_fbb), schema_(_schema){};

Writer::~Writer() = default;

Writer::OutputObjectType Writer::object_as_root(const size_t _size) const {
  if (!schema_->root_type_.type_ptr) {
    throw std::runtime_error("Root type not set.");
  }
  return OutputObjectType{
      .schema_ =
          schema_->root_type_.type_ptr->convert_to<schema::Type::Table>(),
      .offset_ = fbb_->StartTable()};
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {}

Writer::OutputArrayType Writer::add_array_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const {
  const auto schema = _parent->schema_.fields.at(_parent->ix_)
                          .second.convert_to<schema::Type::Vector>();
  start_vector(*schema.type, _size);
  return OutputArrayType{.schema_ = schema};
}

Writer::OutputArrayType Writer::add_array_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {}

Writer::OutputMapType Writer::add_map_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {}

Writer::OutputMapType Writer::add_map_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {}

Writer::OutputMapType Writer::add_map_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {}

Writer::OutputMapType Writer::add_map_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {}

Writer::OutputObjectType Writer::add_object_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {}

Writer::OutputUnionType Writer::add_union_to_array(
    OutputArrayType* _parent) const noexcept {}

Writer::OutputUnionType Writer::add_union_to_map(
    const std::string_view& _name, OutputMapType* _parent) const noexcept {}

Writer::OutputUnionType Writer::add_union_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {}

Writer::OutputVarType Writer::add_null_to_array(
    OutputArrayType* _parent) const noexcept {}

Writer::OutputVarType Writer::add_null_to_map(
    const std::string_view& _name, OutputMapType* _parent) const noexcept {}

Writer::OutputVarType Writer::add_null_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {}

Writer::OutputVarType Writer::add_null_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {}

void Writer::start_vector(const schema::Type& _type, const size_t _size) const {
  _type.reflection().visit([&]<class T>(const T& _t) {
    using U = std::remove_cvref_t<T>;
    if constexpr (std::is_same<U, schema::Type::Bool>()) {
      fbb_->StartVector<bool>(_size);
    } else if constexpr (std::is_same<U, schema::Type::Byte>()) {
      throw std::runtime_error("TODO");  // TODO
    } else if constexpr (std::is_same<U, schema::Type::UByte>()) {
      throw std::runtime_error("TODO");  // TODO
    } else if constexpr (std::is_same<U, schema::Type::Int8>()) {
      fbb_->StartVector<int8_t>(_size);
    } else if constexpr (std::is_same<U, schema::Type::Int16>()) {
      fbb_->StartVector<int16_t>(_size);
    } else if constexpr (std::is_same<U, schema::Type::Int32>()) {
      fbb_->StartVector<int32_t>(_size);
    } else if constexpr (std::is_same<U, schema::Type::Int64>()) {
      fbb_->StartVector<int64_t>(_size);
    } else if constexpr (std::is_same<U, schema::Type::UInt8>()) {
      fbb_->StartVector<uint8_t>(_size);
    } else if constexpr (std::is_same<U, schema::Type::UInt16>()) {
      fbb_->StartVector<uint16_t>(_size);
    } else if constexpr (std::is_same<U, schema::Type::UInt32>()) {
      fbb_->StartVector<uint32_t>(_size);
    } else if constexpr (std::is_same<U, schema::Type::UInt64>()) {
      fbb_->StartVector<uint64_t>(_size);
    } else if constexpr (std::is_same<U, schema::Type::Float32>()) {
      fbb_->StartVector<float>(_size);
    } else if constexpr (std::is_same<U, schema::Type::Float64>()) {
      fbb_->StartVector<double>(_size);
    } else if constexpr (std::is_same<U, schema::Type::String>()) {
      fbb_->StartVector<flatbuffers::Offset<flatbuffers::String>>(_size);
    } else if constexpr (std::is_same<U, schema::Type::Enum>()) {
      throw std::runtime_error("TODO");  // TODO
    } else if constexpr (std::is_same<U, schema::Type::Map>()) {
      throw std::runtime_error("TODO");  // TODO
    } else if constexpr (std::is_same<U, schema::Type::Optional>()) {
      throw std::runtime_error("TODO");  // TODO
    } else if constexpr (std::is_same<U, schema::Type::Vector>()) {
      throw std::runtime_error("TODO");  // TODO
    } else if constexpr (std::is_same<U, schema::Type::Reference>()) {
      if (!_t.type_ptr) {
        throw std::runtime_error("type_ptr not set for '" + _t.type_name +
                                 "'.");
      }
      start_vector(*_t.type_ptr, _size);
    } else if constexpr (std::is_same<U, schema::Type::Table>()) {
      throw std::runtime_error("TODO");  // TODO
    } else if constexpr (std::is_same<U, schema::Type::Union>()) {
      throw std::runtime_error("TODO");  // TODO
    } else {
      static_assert(always_false_v<T>, "Unsupported type");
    }
  });
}

}  // namespace rfl::flatbuf
