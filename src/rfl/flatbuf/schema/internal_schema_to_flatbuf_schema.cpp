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

namespace rfl::flatbuf::schema {

Type type_to_flatbuf_schema_type(
    const parsing::schema::Type& _type,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufTypes* _flatbuf_types);

Type any_of_to_flatbuf_schema_type(
    const parsing::schema::Type::AnyOf& _any_of,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufTypes* _flatbuf_types) {
  auto value = Type::Union{};
  size_t i = 1;
  for (const auto& type : _any_of.types_) {
    value.fields.push_back(
        std::make_pair(std::string("Opt" + std::to_string(i++)),
                       type_to_flatbuf_schema_type(type, _definitions, false,
                                                   _flatbuf_types)));
  }
  value.name =
      std::string("Union") + std::to_string(_flatbuf_types->unions_.size() + 1);
  if (_is_top_level) {
    return Type{.value = value};
  } else {
    _flatbuf_types->unions_[value.name] = Type{.value = value};
    return Type{.value = schema::Type::Reference{name}};
  }
}

Type literal_to_flatbuf_schema_type(
    const parsing::schema::Type::Literal& _literal,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufTypes* _flatbuf_types) {
  const auto enum_schema = Type::Enum{.fields = _literal.values_};
  if (_is_top_level) {
    return Type{.value = enum_schema};
  } else {
    const auto name =
        std::string("Enum") + std::to_string(_flatbuf_types->enums_.size() + 1);
    _flatbuf_types->enums_[name] = Type{.value = enum_schema};
    return Type{.value = schema::Type::Reference{name}};
  }
}

FlatbufTypes internal_schema_to_flatbuf_schema(
    const parsing::schema::Definition& _internal_schema) {
  FlatbufTypes flatbuf_types;
  for (const auto& [name, def] : _internal_schema.definitions_) {
    if (!is_named_type(def)) {
      continue;
    }
    flatbuf_types.structs_[name] = type_to_flatbuf_schema_type(
        def, _internal_schema.definitions_, true, &flatbuf_types);
  }
  return flatbuf_types;
}

Type object_to_flatbuf_schema_type(
    const parsing::schema::Type::Object& _obj,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufTypes* _flatbuf_types) {
  Type::Struct struct_schema;
  for (const auto& [k, v] : _obj.types_) {
    struct_schema.fields.push_back(std::make_pair(
        k,
        type_to_flatbuf_schema_type(v, _definitions, false, _flatbuf_types)));
  }
  if (_is_top_level) {
    return Type{.value = struct_schema};
  } else {
    const auto name = std::string("Tuple") +
                      std::to_string(_flatbuf_types->tuples_.size() + 1);
    _flatbuf_types->tuples_[name] = Type{.value = struct_schema};
    return Type{.value = Type::Reference{name}};
  }
}

Type optional_to_flatbuf_schema_type(
    const parsing::schema::Type::Optional& _optional,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufTypes* _flatbuf_types) {
  return Type{.value = Type::Optional{
                  .type = make_ref<Type>(type_to_flatbuf_schema_type(
                      *_optional.type_, _definitions, false, _flatbuf_types))}};
}

Type reference_to_flatbuf_schema_type(
    const parsing::schema::Type::Reference& _reference,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufTypes* _flatbuf_types) {
  const auto it = _definitions.find(_reference.name_);
  if (it == _definitions.end() || is_named_type(it->second)) {
    return Type{.value = Type::Reference{.type_name = _reference.name_}};
  } else {
    return type_to_flatbuf_schema_type(it->second, _definitions, _is_top_level,
                                       _flatbuf_types);
  }
}

Type type_to_flatbuf_schema_type(
    const parsing::schema::Type& _type,
    const std::map<std::string, parsing::schema::Type>& _definitions,
    const bool _is_top_level, FlatbufTypes* _flatbuf_types) {
  return _type.variant_.visit([&](const auto& _t) -> schema::Type {
    using T = std::remove_cvref_t<decltype(_t)>;
    using Type = parsing::schema::Type;
    if constexpr (std::is_same<T, Type::Boolean>()) {
      return schema::Type{.value = schema::Type::Bool{}};

    } else if constexpr (std::is_same<T, Type::Bytestring>()) {
      // TODO: Implement

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
                                           _flatbuf_types);

    } else if constexpr (std::is_same<T, Type::Description>()) {
      return type_to_flatbuf_schema_type(*_t.type_, _definitions, _is_top_level,
                                         _flatbuf_types);

    } else if constexpr (std::is_same<T, Type::FixedSizeTypedArray>()) {
      return schema::Type{
          .value = schema::Type::Vector{
              .type = Ref<schema::Type>::make(type_to_flatbuf_schema_type(
                  *_t.type_, _definitions, false, _flatbuf_types))}};

    } else if constexpr (std::is_same<T, Type::Literal>()) {
      return literal_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                            _flatbuf_types);

    } else if constexpr (std::is_same<T, Type::Object>()) {
      return object_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                           _flatbuf_types);

    } else if constexpr (std::is_same<T, Type::Optional>()) {
      return optional_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                             _flatbuf_types);

    } else if constexpr (std::is_same<T, Type::Reference>()) {
      return reference_to_flatbuf_schema_type(_t, _definitions, _is_top_level,
                                              _flatbuf_types);

    } else if constexpr (std::is_same<T, Type::StringMap>()) {
      _flatbuf_types->has_maps_ = true;
      return schema::Type{
          .value = schema::Type::Map{
              .type = Ref<schema::Type>::make(type_to_flatbuf_schema_type(
                  *_t.value_type_, _definitions, false, _flatbuf_types))}};

    } else if constexpr (std::is_same<T, Type::Tuple>()) {
      return type_to_flatbuf_schema_type(
          Type{parsing::schemaful::tuple_to_object(_t)}, _definitions, false,
          _flatbuf_types);

    } else if constexpr (std::is_same<T, Type::TypedArray>()) {
      return schema::Type{
          .value = schema::Type::Vector{
              .type = Ref<schema::Type>::make(type_to_flatbuf_schema_type(
                  *_t.type_, _definitions, false, _flatbuf_types))}};

    } else if constexpr (std::is_same<T, Type::Validated>()) {
      // Flatbuffers knows no validation.
      return type_to_flatbuf_schema_type(*_t.type_, _definitions, _is_top_level,
                                         _flatbuf_types);

    } else {
      static_assert(rfl::always_false_v<T>, "Not all cases were covered.");
    }
  });
}

std::string SchemaImpl::str() const {
  std::stringstream stream;
  stream << schema_;
  return stream.str();
}

}  // namespace rfl::flatbuf::schema
