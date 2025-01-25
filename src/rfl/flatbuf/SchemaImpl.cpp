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

#include "rfl/flatbuf/SchemaImpl.hpp"

#include <sstream>

#include "rfl/flatbuf/schema/FlatbufSchema.hpp"
#include "rfl/flatbuf/schema/internal_schema_to_flatbuf_schema.hpp"

namespace rfl::flatbuf {

SchemaImpl::SchemaImpl(const parsing::schema::Definition& _internal_schema)
    : schema_(schema::internal_schema_to_flatbuf_schema(_internal_schema)) {}

std::string SchemaImpl::str() const {
  std::stringstream stream;
  stream << schema_;
  return stream.str();
}

}  // namespace rfl::flatbuf
