#include "rfl/flatbuf/Reader.hpp"

#include "rfl/parsing/schemaful/IsSchemafulReader.hpp"

namespace rfl::flatbuf {

static_assert(parsing::schemaful::IsSchemafulReader<Reader>,
              "This must be a schemaful reader.");

Reader::Reader(const Ref<flatbuffers::Verifier>& _verifier)
    : verifier_(_verifier) {}

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
  std::cout << "to_object1" << std::endl;
  if (!_var.val_) {
    return Error("Could not cast to a table: Is null.");
  }
  std::cout << "to_object2" << std::endl;
  auto obj = InputObjectType{internal::ptr_cast<const flatbuffers::Table*>(
      apply_ptr_correction(_var.val_))};
  std::cout << "to_object3" << std::endl;
  return obj;
}

rfl::Result<Reader::InputMapType> Reader::to_map(
    const InputVarType& _var) const noexcept {
  // TODO
}

rfl::Result<Reader::InputUnionType> Reader::to_union(
    const InputVarType& _var) const noexcept {
  // TODO
}

}  // namespace rfl::flatbuf
