#include "rfl/capnproto/Reader.hpp"

#include "rfl/parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::capnproto {

static_assert(parsing::schemaful::IsSchemafulReader<Reader>,
              "This must be a schemaful reader.");

bool Reader::is_empty(const InputVarType& _var) const noexcept {
  return !_var.val_;
}

rfl::Result<Reader::InputArrayType> Reader::to_array(
    const InputVarType& _var) const noexcept {
  if (!_var.val_) {
    return Error("Could not cast to a vector: Is null.");
  }
  return InputArrayType{internal::ptr_cast<const flatbuffers::VectorOfAny*>(
      apply_ptr_correction(_var.val_))};
}

rfl::Result<Reader::InputObjectType> Reader::to_object(
    const InputVarType& _var) const noexcept {
  if (!_var.val_) {
    return Error("Could not cast to a table: Is null.");
  }
  return InputObjectType{internal::ptr_cast<const flatbuffers::Table*>(
      apply_ptr_correction(_var.val_))};
}

rfl::Result<Reader::InputMapType> Reader::to_map(
    const InputVarType& _var) const noexcept {
  // TODO
}

rfl::Result<Reader::InputUnionType> Reader::to_union(
    const InputVarType& _var) const noexcept {
  // TODO
}

}  // namespace rfl::capnproto
