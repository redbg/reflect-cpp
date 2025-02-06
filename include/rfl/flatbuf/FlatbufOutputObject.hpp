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
                      flatbuffers::FlatBufferBuilder* _fbb);

  ~FlatbufOutputObject() = default;

  /// Adds a scalar to the object.
  template <class T>
  void add_scalar(const T _val) {
    // TODO
    // fbb_->AddElement<T>(calc_vtable_offset(ix_++), _val);
  }

  /// Adds an offset to the the array.
  void add_offset(const flatbuffers::uoffset_t _offset) final;

  /// Ends the construction of the object.
  void end();

  /// Returns the schema for the current field.
  const schema::Type& get_current_schema() const {
    return schema_.fields.at(offsets_.size() - 1).second;
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

  /// The data.
  std::vector<uint8_t> data_;

  /// The underlying offsets in the data.
  std::vector<uint8_t> offsets_;
};

}  // namespace rfl::flatbuf

#endif
