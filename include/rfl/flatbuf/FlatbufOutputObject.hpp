#ifndef RFL_FLATBUF_FLATBUFOUTPUTOBJECT_HPP_
#define RFL_FLATBUF_FLATBUFOUTPUTOBJECT_HPP_

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/flatbuffers.h>

#include <cstdint>
#include <cstring>
#include <vector>

#include "FlatbufOutputParent.hpp"
#include "calc_vtable_offset.hpp"
#include "schema/Type.hpp"

namespace rfl::flatbuf {

struct FlatbufOutputObject : public FlatbufOutputParent {
  template <class T>
  struct TypeWrapper {};

  FlatbufOutputObject(const schema::Type::Table& _schema,
                      FlatbufOutputParent* _parent,
                      flatbuffers::FlatBufferBuilder* _fbb);

  ~FlatbufOutputObject() = default;

  /// Adds a scalar to the object.
  template <class T>
  void add_scalar(const T _val) {
    static_assert(sizeof(T) <= sizeof(uint64_t),
                  "Size cannot be greater than 4.");
    data_.push_back(0);
    std::memcpy(&data_.back(), &_val, sizeof(T));
  }

  /// Adds an offset to the the array.
  void add_offset(const flatbuffers::uoffset_t _offset) final;

  /// Ends the construction of the object.
  void end();

  /// Returns the schema for the current field.
  const schema::Type& get_current_schema() const {
    return schema_.fields.at(data_.size()).second;
  }

  /// Returns the underlying schema.
  const schema::Type::Table& schema() const { return schema_; }

 private:
  /// Adds an element from data_ to the table to create.
  void add_to_table(const size_t _i, const schema::Type& _type,
                    const uint64_t _val);

 private:
  /// The underlying schema.
  schema::Type::Table schema_;

  /// The parent. nullptr if this is root.
  FlatbufOutputParent* parent_;

  /// Pointer to the underlying flatbuffer builder.
  flatbuffers::FlatBufferBuilder* fbb_;

  /// The data. Note that no type can be greater than 8 bytes.
  std::vector<uint64_t> data_;
};

}  // namespace rfl::flatbuf

#endif
