/*

MIT License

Copyright (c) 2023-2024 Code17 GmbH

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

#include "rfl/flatbuf/schema/Type.hpp"

#include <functional>
#include <type_traits>

#include "rfl/internal/strings/strings.hpp"

namespace rfl::flatbuf::schema {

Type Type::with_name(const std::string& _name) const {
  const auto set_name = [&](const auto& _v) -> ReflectionType {
    using T = std::remove_cvref_t<decltype(_v)>;
    if constexpr (std::is_same<T, Table>() || std::is_same<T, Union>() ||
                  std::is_same<T, Enum>()) {
      auto v_with_name = _v;
      v_with_name.name = _name;
      return v_with_name;
    } else {
      return _v;
    }
  };
  return Type{.value = value.visit(set_name)};
}

std::ostream& operator<<(std::ostream& _os, const Type::Bool&) {
  return _os << "bool";
}

std::ostream& operator<<(std::ostream& _os, const Type::Byte&) {
  return _os << "byte";
}

std::ostream& operator<<(std::ostream& _os, const Type::UByte&) {
  return _os << "ubyte";
}

std::ostream& operator<<(std::ostream& _os, const Type::Int8&) {
  return _os << "int8";
}

std::ostream& operator<<(std::ostream& _os, const Type::Int16&) {
  return _os << "int16";
}

std::ostream& operator<<(std::ostream& _os, const Type::Int32&) {
  return _os << "int32";
}

std::ostream& operator<<(std::ostream& _os, const Type::Int64&) {
  return _os << "int64";
}

std::ostream& operator<<(std::ostream& _os, const Type::UInt8&) {
  return _os << "uint8";
}

std::ostream& operator<<(std::ostream& _os, const Type::UInt16&) {
  return _os << "uint16";
}

std::ostream& operator<<(std::ostream& _os, const Type::UInt32&) {
  return _os << "uint32";
}

std::ostream& operator<<(std::ostream& _os, const Type::UInt64&) {
  return _os << "uint64";
}

std::ostream& operator<<(std::ostream& _os, const Type::Float32&) {
  return _os << "float32";
}

std::ostream& operator<<(std::ostream& _os, const Type::Float64&) {
  return _os << "float64";
}

std::ostream& operator<<(std::ostream& _os, const Type::String&) {
  return _os << "string";
}

std::ostream& operator<<(std::ostream& _os, const Type::Enum& _e) {
  return _os << "enum " << _e.name << ": uint16 { "
             << internal::strings::join(", ", _e.fields) << " }" << std::endl;
}

std::ostream& operator<<(std::ostream& _os, const Type::Vector& _v) {
  return _os << "[" << *_v.type << "]";
}

std::ostream& operator<<(std::ostream& _os, const Type::Map& _m) {
  return _os << "TODO(" << *_m.type << ")";
}

std::ostream& operator<<(std::ostream& _os, const Type::Optional& _o) {
  return _os << "TODO(" << *_o.type << ")";
}

std::ostream& operator<<(std::ostream& _os, const Type::Table& _t) {
  _os << "table " << _t.name << " {" << std::endl;
  for (const auto& f : _t.fields) {
    _os << " " << f.name << ":" << f.type << ";" << std::endl;
  }
  return _os << "}" << std::endl;
}

std::ostream& operator<<(std::ostream& _os, const Type::Union& _u) {
  return _os << "TODO" << std::endl;
}

std::ostream& operator<<(std::ostream& _os, const Type::Reference& _r) {
  return _os << _r.type_name;
}

std::ostream& operator<<(std::ostream& _os, const Type& _t) {
  return _t.reflection().visit(
      [&](const auto& _r) -> std::ostream& { return _os << _r; });
}

}  // namespace rfl::flatbuf::schema
