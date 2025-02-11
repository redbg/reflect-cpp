#ifndef RFL_FLATBUF_FLATBUFOUTPUTUNION_HPP_
#define RFL_FLATBUF_FLATBUFOUTPUTUNION_HPP_

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/flatbuffers.h>

#include <cstdint>
#include <cstring>
#include <vector>

#include "FlatbufOutputParent.hpp"
#include "calc_vtable_offset.hpp"
#include "schema/Type.hpp"

namespace rfl::flatbuf {

struct FlatbufOutputUnion : public FlatbufOutputParent {
  template <class T>
  struct TypeWrapper {};

  FlatbufOutputUnion(const schema::Type::Table& _schema, const size_t _index,
                     FlatbufOutputParent* _parent,
                     flatbuffers::FlatBufferBuilder* _fbb);

  ~FlatbufOutputUnion() = default;

  /// Adds a scalar to the object.
  template <class T>
  void add_scalar(const T _val) {
    static_assert(sizeof(T) <= sizeof(uint64_t),
                  "Size cannot be greater than 4.");
    std::memcpy(&data_, &_val, sizeof(T));
  }

  /// Adds an offset to the the array.
  void add_offset(const flatbuffers::uoffset_t _offset) final;

  /// Ends the construction of the object.
  void end();

  /// Returns the schema for the current field.
  const schema::Type& get_current_schema() const {
    return schema_.fields.at(index_).second;
  }

  /// Returns the underlying schema.
  const schema::Type::Table& schema() const { return schema_; }

 private:
  /// The underlying schema.
  schema::Type::Table schema_;

  /// Indicates which of the options is currently active.
  size_t index_;

  /// The parent. nullptr if this is root.
  FlatbufOutputParent* parent_;

  /// Pointer to the underlying flatbuffer builder.
  flatbuffers::FlatBufferBuilder* fbb_;

  /// The data. Note that no type can be greater than 8 bytes.
  uint64_t data_;
};

}  // namespace rfl::flatbuf

#endif
