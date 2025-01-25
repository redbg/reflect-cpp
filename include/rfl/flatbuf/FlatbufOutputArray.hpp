#ifndef RFL_FLATBUF_FLATBUFOUTPUTARRAY_HPP_
#define RFL_FLATBUF_FLATBUFOUTPUTARRAY_HPP_

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/flatbuffers.h>

#include <cstdint>
#include <vector>

#include "FlatbufOutputParent.hpp"
#include "schema/Type.hpp"

namespace rfl::flatbuf {

/// The flatbuffers API is a bit awkward, so we build these wrapper types around
/// it.
struct FlatbufOutputArray : public FlatbufOutputParent {
  template <class T>
  struct TypeWrapper {};

  FlatbufOutputArray(const schema::Type::Vector& _schema,
                     FlatbufOutputParent* _parent,
                     flatbuffers::FlatBufferBuilder* _fbb)
      : schema_(_schema), parent_(_parent), fbb_(_fbb) {}

  ~FlatbufOutputArray() = default;

  /// Adds a scalar to the array.
  template <class T>
  void add_scalar(const T _val) {
    auto ptr = internal::ptr_cast<const uint8_t*>(&_val);
    data_.insert(data_.end(), ptr, ptr + sizeof(T));
  }

  /// Adds an offset to the the array.
  void add_offset(const flatbuffers::uoffset_t _offset) final {
    auto offset = flatbuffers::Offset<>(_offset);
    const auto ptr = internal::ptr_cast<const uint8_t*>(&offset);
    data_.insert(data_.end(), ptr, ptr + sizeof(flatbuffers::Offset<>));
  }

  /// Ends the vector by adding it to the builder.
  void end() {
    const auto offset = build_vector(*schema_.type);
    if (parent_) {
      parent_->add_offset(offset);
    }
  }

  /// Returns the underlying schema.
  const schema::Type::Vector& schema() const { return schema_; }

 private:
  /// Builds the vector using the builder.
  flatbuffers::uoffset_t build_vector(const schema::Type& _type) const;

 private:
  /// The underlying schema.
  schema::Type::Vector schema_;

  /// The parent. nullptr if this is root.
  FlatbufOutputParent* parent_;

  /// Pointer to the underlying flatbuffer builder.
  flatbuffers::FlatBufferBuilder* fbb_;

  /// The data/
  std::vector<uint8_t> data_;
};

}  // namespace rfl::flatbuf

#endif
