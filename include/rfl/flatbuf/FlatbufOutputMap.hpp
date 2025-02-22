#ifndef RFL_FLATBUF_FLATBUFOUTPUTMAP_HPP_
#define RFL_FLATBUF_FLATBUFOUTPUTMAP_HPP_

#include <flatbuffers/flatbuffers.h>

#include <cstdint>
#include <vector>

#include "FlatbufOutputParent.hpp"

namespace rfl::flatbuf {

struct FlatbufOutputMap : public FlatbufOutputParent {
  FlatbufOutputMap(const schema::Type::Vector& _schema,
                   FlatbufOutputParent* _parent)
      : schema_(_schema), parent_(_parent), ix_(0), elem_size_(1 /*TODO*/) {}

  ~FlatbufOutputMap() = default;

  /// Adds an offset to the the array.
  void add_offset(const flatbuffers::uoffset_t _offset) final {
    *internal::ptr_cast<flatbuffers::Offset<>*>(
        data_.data() + elem_size_ * ix_) = flatbuffers::Offset<>(_offset);
  }

  /// Returns the underlying data.
  const std::vector<uint8_t>& data() const { return data_; }

  /// Returns the underlying schema.
  const schema::Type::Vector& schema() const { return schema_; }

 private:
  /// The underlying schema.
  schema::Type::Vector schema_;

  /// The parent. nullptr if this is root.
  FlatbufOutputParent* parent_;

  /// The current index of the vector.
  size_t ix_;

  /// The size of the underlying element
  size_t elem_size_;

  /// The data/
  std::vector<uint8_t> data_;
};

}  // namespace rfl::flatbuf

#endif
