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

#include "rfl/flatbuf/schema/internal_schema_to_flatbuf_schema.hpp"

#include <sstream>
#include <stdexcept>
#include <type_traits>

#include "rfl/flatbuf/is_named_type.hpp"
#include "rfl/parsing/schema/Definition.hpp"
#include "rfl/parsing/schemaful/tuple_to_object.hpp"

namespace rfl::flatbuf::schema {

Type type_to_flatbuf_schema_type(
    const parsing::schema::Type& _type,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufSchema* _flatbuf_schema);

Type any_of_to_flatbuf_schema_type(
    const parsing::schema::Type::AnyOf& _any_of,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufSchema* _flatbuf_schema) {
  auto value =
      Type::Union{.name = std::string("Union") +
                          std::to_string(_flatbuf_schema->unions_->size() + 1)};
  size_t i = 1;
  for (const auto& type : _any_of.types_) {
    value.fields.push_back(
        std::make_pair(std::string("Opt" + std::to_string(i++)),
                       type_to_flatbuf_schema_type(type, _definitions, false,
                                                   _flatbuf_schema)));
  }
  if (_is_top_level) {
    return Type{.value = value};
  } else {
    const auto type = Type{value};
    (*_flatbuf_schema->unions_)[value.name] = type;
    return Type{.value = schema::Type::Reference{.type_name = value.name}};
  }
}

Type literal_to_flatbuf_schema_type(
    const parsing::schema::Type::Literal& _literal,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufSchema* _flatbuf_schema) {
  const auto enum_schema = Type::Enum{.fields = _literal.values_};
  if (_is_top_level) {
    return Type{.value = enum_schema};
  } else {
    const auto name = std::string("Enum") +
                      std::to_string(_flatbuf_schema->enums_->size() + 1);
    const auto type = Type{enum_schema};
    (*_flatbuf_schema->enums_)[name] = type;
    return Type{.value = schema::Type::Reference{.type_name = name}};
  }
}

Type::Reference make_root_type(const parsing::schema::Type& _t) {
  return _t.variant_.visit([](const auto& _r) {
    using T = std::remove_cvref_t<decltype(_r)>;
    if constexpr (std::is_same<T, parsing::schema::Type::Reference>()) {
      return Type::Reference{.type_name = _r.name_};
    } else {
      return Type::Reference{.type_name = "Error: Root type must be a struct."};
    }
  });
}

Result<FlatbufSchema> internal_schema_to_flatbuf_schema(
    const parsing::schema::Definition& _internal_schema) {
  FlatbufSchema flatbuf_schema{.root_type_ =
                                   make_root_type(_internal_schema.root_)};
  for (const auto& [name, def] : _internal_schema.definitions_) {
    if (!is_named_type(def)) {
      continue;
    }
    (*flatbuf_schema.structs_)[name] = type_to_flatbuf_schema_type(
        def, _internal_schema.definitions_, true, &flatbuf_schema);
  }
  return flatbuf_schema.set_reference_ptrs();
}

Type object_to_flatbuf_schema_type(
    const parsing::schema::Type::Object& _obj,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufSchema* _flatbuf_schema) {
  Type::Table table_schema;
  for (const auto& [k, v] : _obj.types_) {
    table_schema.fields.push_back(std::make_pair(
        k,
        type_to_flatbuf_schema_type(v, _definitions, false, _flatbuf_schema)));
  }
  if (_is_top_level) {
    return Type{.value = table_schema};
  } else {
    const auto type = Type{table_schema};
    const auto name = std::string("Tuple") +
                      std::to_string(_flatbuf_schema->tuples_->size() + 1);
    (*_flatbuf_schema->tuples_)[name] = type;
    return Type{.value = Type::Reference{.type_name = name}};
  }
}

Type optional_to_flatbuf_schema_type(
    const parsing::schema::Type::Optional& _optional,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufSchema* _flatbuf_schema) {
  return Type{
      .value = Type::Optional{
          .type = make_ref<Type>(type_to_flatbuf_schema_type(
              *_optional.type_, _definitions, false, _flatbuf_schema))}};
}

Type reference_to_flatbuf_schema_type(
    const parsing::schema::Type::Reference& _reference,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufSchema* _flatbuf_schema) {
  const auto it = _definitions.find(_reference.name_);
  if (it == _definitions.end() || is_named_type(it->second)) {
    return Type{.value = Type::Reference{.type_name = _reference.name_}};
  } else {
    return type_to_flatbuf_schema_type(it->second, _definitions, _is_top_level,
                                       _flatbuf_schema);
  }
}

Type type_to_flatbuf_schema_type(
    const parsing::schema::Type& _type,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufSchema* _flatbuf_schema) {
  return _type.variant_.visit([&](const auto& _t) -> schema::Type {
    using T = std::remove_cvref_t<decltype(_t)>;
    using Type = parsing::schema::Type;
    if constexpr (std::is_same<T, Type::Boolean>()) {
      return schema::Type{.value = schema::Type::Bool{}};

    } else if constexpr (std::is_same<T, Type::Bytestring>()) {
      // TODO: Implement
      return schema::Type{.value = schema::Type::String{}};

    } else if constexpr (std::is_same<T, Type::Int32>() ||
                         std::is_same<T, Type::Integer>()) {
      return schema::Type{.value = schema::Type::Int32{}};

    } else if constexpr (std::is_same<T, Type::Int64>()) {
      return schema::Type{.value = schema::Type::Int64{}};

    } else if constexpr (std::is_same<T, Type::UInt32>()) {
      return schema::Type{.value = schema::Type::UInt32{}};

    } else if constexpr (std::is_same<T, Type::UInt64>()) {
      return schema::Type{.value = schema::Type::UInt64{}};

    } else if constexpr (std::is_same<T, Type::Float>()) {
      return schema::Type{.value = schema::Type::Float32{}};

    } else if constexpr (std::is_same<T, Type::Double>()) {
      return schema::Type{.value = schema::Type::Float64{}};

    } else if constexpr (std::is_same<T, Type::String>()) {
      return schema::Type{.value = schema::Type::String{}};

    } else if constexpr (std::is_same<T, Type::AnyOf>()) {
      return any_of_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                           _flatbuf_schema);

    } else if constexpr (std::is_same<T, Type::Description>()) {
      return type_to_flatbuf_schema_type(*_t.type_, _definitions, _is_top_level,
                                         _flatbuf_schema);

    } else if constexpr (std::is_same<T, Type::FixedSizeTypedArray>()) {
      return schema::Type{
          .value = schema::Type::Vector{
              .type = Ref<schema::Type>::make(type_to_flatbuf_schema_type(
                  *_t.type_, _definitions, false, _flatbuf_schema))}};

    } else if constexpr (std::is_same<T, Type::Literal>()) {
      return literal_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                            _flatbuf_schema);

    } else if constexpr (std::is_same<T, Type::Object>()) {
      return object_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                           _flatbuf_schema);

    } else if constexpr (std::is_same<T, Type::Optional>()) {
      return optional_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                             _flatbuf_schema);

    } else if constexpr (std::is_same<T, Type::Reference>()) {
      return reference_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                              _flatbuf_schema);

    } else if constexpr (std::is_same<T, Type::StringMap>()) {
      return schema::Type{
          .value = schema::Type::Map{
              .type = Ref<schema::Type>::make(type_to_flatbuf_schema_type(
                  *_t.value_type_, _definitions, false, _flatbuf_schema))}};

    } else if constexpr (std::is_same<T, Type::Tuple>()) {
      return type_to_flatbuf_schema_type(
          Type{parsing::schemaful::tuple_to_object(_t)}, _definitions, false,
          _flatbuf_schema);

    } else if constexpr (std::is_same<T, Type::TypedArray>()) {
      return schema::Type{
          .value = schema::Type::Vector{
              .type = Ref<schema::Type>::make(type_to_flatbuf_schema_type(
                  *_t.type_, _definitions, false, _flatbuf_schema))}};

    } else if constexpr (std::is_same<T, Type::Validated>()) {
      // Flatbuffers knows no validation.
      return type_to_flatbuf_schema_type(*_t.type_, _definitions, _is_top_level,
                                         _flatbuf_schema);

    } else {
      static_assert(rfl::always_false_v<T>, "Not all cases were covered.");
    }
  });
}

}  // namespace rfl::flatbuf::schema
