
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

#include "rfl/flatbuf/schema/FlatbufTypes.hpp"

namespace rfl::flatbuf::schema {

std::ostream& operator<<(std::ostream& _os,
                         const std::map<std::string, Type>& _map) {
  for (const auto& [name, type] : _map) {
    _os << type.with_name(name) << std::endl << std::endl;
  }
  return _os;
}

std::ostream& operator<<(std::ostream& _os,
                         const FlatbufTypes& _flatbuf_types) {
  _os << _flatbuf_types.structs_ << _flatbuf_types.enums_
      << _flatbuf_types.tuples_ << _flatbuf_types.unions_ << "root_type "
      << _flatbuf_types.root_type_ << ";";
  return _os;
}

}  // namespace rfl::flatbuf::schema
