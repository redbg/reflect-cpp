
/*

MIT License

Copyright (c) 2023-2025 Code17 GmbH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "rfl/flatbuf/schema/FlatbufSchema.hpp"

namespace rfl::flatbuf::schema {

std::ostream& operator<<(std::ostream& _os,
                         const std::map<std::string, Type>& _map) {
  for (const auto& [name, type] : _map) {
    _os << type.with_name(name) << std::endl << std::endl;
  }
  return _os;
}

std::ostream& operator<<(std::ostream& _os,
                         const FlatbufSchema& _flatbuf_schema) {
  _os << *_flatbuf_schema.structs_ << *_flatbuf_schema.enums_
      << *_flatbuf_schema.tuples_ << *_flatbuf_schema.unions_ << "root_type "
      << _flatbuf_schema.root_type_ << ";";
  return _os;
}

const schema::Type* FlatbufSchema::find_in_schema(const FlatbufSchema& _schema,
                                                  const std::string& _name) {
  auto it = _schema.structs_->find(_name);
  if (it != _schema.structs_->end()) [[likely]] {
    return &it->second;
  }
  it = _schema.enums_->find(_name);
  if (it != _schema.enums_->end()) {
    return &it->second;
  }
  it = _schema.tuples_->find(_name);
  if (it != _schema.tuples_->end()) {
    return &it->second;
  }
  it = _schema.unions_->find(_name);
  if (it != _schema.unions_->end()) {
    return &it->second;
  }
  throw std::runtime_error("Could not find reference to '" + _name + "'.");
}

void FlatbufSchema::set_reference_ptrs_on_type(const FlatbufSchema& _schema,
                                               Type* _type) {
  _type->value.visit([&](auto& _t) {
    using T = std::remove_cvref_t<decltype(_t)>;
    if constexpr (std::is_same<T, Type::Reference>()) {
      _t.type_ptr = find_in_schema(_schema, _t.type_name);
    } else if constexpr (std::is_same<T, Type::Optional>() ||
                         std::is_same<T, Type::Vector>() ||
                         std::is_same<T, Type::Map>()) {
      set_reference_ptrs_on_type(_schema, _t.type.get());
    } else if constexpr (std::is_same<T, Type::Table>() ||
                         std::is_same<T, Type::Union>()) {
      for (auto& f : _t.fields) {
        set_reference_ptrs_on_type(_schema, &f.second);
      }
    }
  });
}

void FlatbufSchema::set_reference_ptrs_on_map(
    const FlatbufSchema& _schema, std::map<std::string, schema::Type>* _map) {
  for (auto& [k, v] : *_map) {
    set_reference_ptrs_on_type(_schema, &v);
  }
}

}  // namespace rfl::flatbuf::schema
