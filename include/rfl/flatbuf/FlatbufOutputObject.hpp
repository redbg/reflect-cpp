#ifndef RFL_FLATBUF_FLATBUFOUTPUTOBJECT_HPP_
#define RFL_FLATBUF_FLATBUFOUTPUTOBJECT_HPP_

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/flatbuffers.h>

#include <cstdint>
#include <vector>

#include "FlatbufOutputParent.hpp"
#include "calc_vtable_offset.hpp"
#include "schema/Type.hpp"

namespace rfl::flatbuf {

struct FlatbufOutputObject : public FlatbufOutputParent {
  FlatbufOutputObject(const schema::Type::Table& _schema,
                      FlatbufOutputParent* _parent,
                      flatbuffers::FlatBufferBuilder* _fbb)
      : schema_(_schema),
        parent_(_parent),
        fbb_(_fbb),
        ix_(0),
        start_(fbb_->StartTable()) {}

  ~FlatbufOutputObject() = default;

  /// Adds a scalar to the object.
  template <class T>
  void add_scalar(const T _val) {
    fbb_->AddElement<T>(calc_vtable_offset(ix_++), _val);
  }

  /// Adds an offset to the the array.
  void add_offset(const flatbuffers::uoffset_t _offset) final {
    fbb_->AddOffset<>(calc_vtable_offset(ix_++),
                      flatbuffers::Offset<>(_offset));
  }

  /// Ends the construction of the object.
  void end() {
    auto offset = fbb_->EndTable(start_);
    if (parent_) {
      parent_->add_offset(offset);
    }
  }

  /// Returns the schema for the current field.
  const schema::Type& get_current_schema() const {
    return schema_.fields.at(ix_).second;
  }

  /// Returns the underlying schema.
  const schema::Type::Table& schema() const { return schema_; }

 private:
  /// The underlying schema.
  schema::Type::Table schema_;

  /// The parent. nullptr if this is root.
  FlatbufOutputParent* parent_;

  /// Pointer to the underlying flatbuffer builder.
  flatbuffers::FlatBufferBuilder* fbb_;

  /// Indicates the current field.
  size_t ix_;

  /// The offset of the start.
  flatbuffers::uoffset_t start_;
};

}  // namespace rfl::flatbuf

#endif
