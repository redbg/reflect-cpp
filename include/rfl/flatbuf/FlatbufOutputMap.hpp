#ifndef RFL_FLATBUF_FLATBUFOUTPUTMAP_HPP_
#define RFL_FLATBUF_FLATBUFOUTPUTMAP_HPP_

#include <flatbuffers/flatbuffers.h>

#include <cstdint>
#include <string_view>
#include <vector>

#include "FlatbufOutputParent.hpp"

namespace rfl::flatbuf {

struct FlatbufOutputMap : public FlatbufOutputParent {
  FlatbufOutputMap(const schema::Type::Table& _schema,
                   FlatbufOutputParent* _parent,
                   flatbuffers::FlatBufferBuilder* _fbb)
      : schema_(_schema), parent_(_parent), fbb_(_fbb) {}

  ~FlatbufOutputMap() = default;

  /// Adds a key to the keys.
  void add_key(const std::string_view& _key);

  /// Adds a scalar to the array.
  template <class T>
  void add_scalar(const T _val) {
    auto ptr = internal::ptr_cast<const uint8_t*>(&_val);
    values_.insert(values_.end(), ptr, ptr + sizeof(T));
  }

  /// Adds an offset to the the array.
  void add_offset(const flatbuffers::uoffset_t _offset) final;

  /// Returns the underlying schema.
  const schema::Type::Table& schema() const { return schema_; }

  /// Returns the schema of the underlying value
  const schema::Type value_schema() const {
    return schema_.fields.at(1).second;
  }

 private:
  /// The underlying schema.
  schema::Type::Table schema_;

  /// The parent. nullptr if this is root.
  FlatbufOutputParent* parent_;

  /// Pointer to the underlying flatbuffer builder.
  flatbuffers::FlatBufferBuilder* fbb_;

  /// The keys
  std::vector<flatbuffers::Offset<>> keys_;

  /// The values
  std::vector<uint8_t> values_;
};

}  // namespace rfl::flatbuf

#endif
