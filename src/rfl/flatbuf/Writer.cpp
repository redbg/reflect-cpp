#include "rfl/flatbuf/Writer.hpp"

#include <stdexcept>

#include "rfl/always_false.hpp"
#include "rfl/parsing/schemaful/IsSchemafulWriter.hpp"

namespace rfl::flatbuf {

static_assert(parsing::schemaful::IsSchemafulWriter<Writer>,
              "This must be a schemaful writer.");

Writer::Writer(const Ref<flatbuffers::FlatBufferBuilder>& _fbb,
               const schema::FlatbufSchema& _schema)
    : fbb_(_fbb), schema_(_schema){};

Writer::~Writer() = default;

Writer::OutputObjectType Writer::object_as_root(const size_t _size) const {
  if (!schema_.root_type_.type_ptr) {
    throw std::runtime_error("Root type ptr not set.");
  }
  return OutputObjectType(
      schema_.root_type_.type_ptr->convert_to<schema::Type::Table>(), nullptr,
      fbb_.get());
}

Writer::OutputArrayType Writer::add_array_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return OutputArrayType(
      _parent->schema().type->convert_to<schema::Type::Vector>(), _parent,
      fbb_.get());
}

Writer::OutputArrayType Writer::add_array_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  _parent->add_key(_name);
  return OutputArrayType(
      _parent->value_schema().convert_to<schema::Type::Vector>(), _parent,
      fbb_.get());
}

Writer::OutputArrayType Writer::add_array_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const {
  return OutputArrayType(
      _parent->get_current_schema().convert_to<schema::Type::Vector>(), _parent,
      fbb_.get());
}

Writer::OutputArrayType Writer::add_array_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  _parent->set_index(_index);
  return OutputArrayType(_parent->get_current_schema()
                             .convert_to<schema::Type::Table>()
                             .fields.at(0)
                             .second.convert_to<schema::Type::Vector>(),
                         _parent, fbb_.get());
}

Writer::OutputMapType Writer::add_map_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {}

Writer::OutputMapType Writer::add_map_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  _parent->add_key(_name);
  return OutputMapType(
      _parent->value_schema().convert_to<schema::Type::Table>(), _parent,
      fbb_.get());
}

Writer::OutputMapType Writer::add_map_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {}

Writer::OutputMapType Writer::add_map_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {}

Writer::OutputObjectType Writer::add_object_to_array(
    const size_t _size, OutputArrayType* _parent) const noexcept {
  return OutputObjectType(
      _parent->schema().type->convert_to<schema::Type::Table>(), _parent,
      fbb_.get());
}

Writer::OutputObjectType Writer::add_object_to_map(
    const std::string_view& _name, const size_t _size,
    OutputMapType* _parent) const noexcept {
  _parent->add_key(_name);
  return OutputObjectType(
      _parent->value_schema().convert_to<schema::Type::Table>(), _parent,
      fbb_.get());
}

Writer::OutputObjectType Writer::add_object_to_object(
    const std::string_view& _name, const size_t _size,
    OutputObjectType* _parent) const noexcept {
  return OutputObjectType(
      _parent->get_current_schema().convert_to<schema::Type::Table>(), _parent,
      fbb_.get());
}

Writer::OutputObjectType Writer::add_object_to_union(
    const size_t _index, const size_t _size,
    OutputUnionType* _parent) const noexcept {
  _parent->set_index(_index);
  return OutputObjectType(_parent->get_current_schema()
                              .convert_to<schema::Type::Table>()
                              .fields.at(0)
                              .second.convert_to<schema::Type::Table>(),
                          _parent, fbb_.get());
}

Writer::OutputUnionType Writer::add_union_to_array(
    OutputArrayType* _parent) const noexcept {
  return OutputUnionType(_parent->schema()
                             .type->convert_to<schema::Type::Table>()
                             .fields.at(0)
                             .second.convert_to<schema::Type::Union>(),
                         _parent, fbb_.get());
}

Writer::OutputUnionType Writer::add_union_to_map(
    const std::string_view& _name, OutputMapType* _parent) const noexcept {
  _parent->add_key(_name);
  return OutputUnionType(_parent->value_schema()
                             .convert_to<schema::Type::Table>()
                             .fields.at(0)
                             .second.convert_to<schema::Type::Union>(),
                         _parent, fbb_.get());
}

Writer::OutputUnionType Writer::add_union_to_object(
    const std::string_view& _name, OutputObjectType* _parent) const noexcept {
  return OutputUnionType(_parent->get_current_schema()
                             .convert_to<schema::Type::Table>()
                             .fields.at(0)
                             .second.convert_to<schema::Type::Union>(),
                         _parent, fbb_.get());
}

Writer::OutputUnionType Writer::add_union_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  _parent->set_index(_index);
  return OutputUnionType(_parent->get_current_schema()
                             .convert_to<schema::Type::Table>()
                             .fields.at(0)
                             .second.convert_to<schema::Type::Table>()
                             .fields.at(0)
                             .second.convert_to<schema::Type::Union>(),
                         _parent, fbb_.get());
}

Writer::OutputVarType Writer::add_null(
    FlatbufOutputParent* _parent) const noexcept {
  const auto start = fbb_->StartTable();
  const auto offset = fbb_->EndTable(start);
  _parent->add_offset(offset);
  return OutputVarType{};
}

Writer::OutputVarType Writer::add_null_to_union(
    const size_t _index, OutputUnionType* _parent) const noexcept {
  _parent->set_index(_index);
  return add_null(_parent);
}

}  // namespace rfl::flatbuf
