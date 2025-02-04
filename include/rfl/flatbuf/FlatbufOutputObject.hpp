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
      : schema_(_schema), parent_(_parent), fbb_(_fbb) {
    sizes_.push_back(0);
  }

  ~FlatbufOutputObject() = default;

  /// Adds a scalar to the object.
  template <class T>
  void add_scalar(const T _val) {
    // TODO
    // fbb_->AddElement<T>(calc_vtable_offset(ix_++), _val);
  }

  /// Adds an offset to the the array.
  void add_offset(const flatbuffers::uoffset_t _offset) final {
    // fbb_->AddOffset<>(calc_vtable_offset(ix_++),
    //                   flatbuffers::Offset<>(_offset));
    auto offset = flatbuffers::Offset<>(_offset);
    const auto ptr = internal::ptr_cast<const uint8_t*>(&offset);
    data_.insert(data_.end(), ptr, ptr + sizeof(flatbuffers::Offset<>));
    sizes_.push_back(sizes_.back() + sizeof(flatbuffers::Offset<>));
  }

  /// Ends the construction of the object.
  void end() {
    const auto start = fbb_->StartTable();
    for (size_t i = 0; i < sizes_.size(); ++i) {
      fbb_->AddOffset<>(calc_vtable_offset(i),
                        *internal::ptr_cast<flatbuffers::Offset<>*>(
                            data_.data() + sizes_[i]));
    }
    auto offset = fbb_->EndTable(start);
    if (parent_) {
      parent_->add_offset(offset);
    } else {
      fbb_->Finish(flatbuffers::Offset<>(offset));
    }
  }

  /// Returns the schema for the current field.
  const schema::Type& get_current_schema() const {
    return schema_.fields.at(sizes_.size() - 1).second;
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

  /// The underlying sizes.
  std::vector<uint8_t> sizes_;
};

}  // namespace rfl::flatbuf

#endif
