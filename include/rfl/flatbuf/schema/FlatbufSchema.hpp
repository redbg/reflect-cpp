#ifndef RFL_FLATBUF_SCHEMA_FLATBUFSCHEMA_HPP_
#define RFL_FLATBUF_SCHEMA_FLATBUFSCHEMA_HPP_

#include <map>

#include "../../Ref.hpp"
#include "../../Result.hpp"
#include "Type.hpp"

namespace rfl::flatbuf::schema {

struct FlatbufSchema {
 public:
  Ref<std::map<std::string, schema::Type>> structs_;
  Ref<std::map<std::string, schema::Type>> enums_;
  Ref<std::map<std::string, schema::Type>> tuples_;
  Ref<std::map<std::string, schema::Type>> unions_;
  Ref<std::map<std::string, schema::Type>> union_helpers_;

  Type::Reference root_type_;

  Result<FlatbufSchema> set_reference_ptrs() const {
    auto schema = *this;
    try {
      set_reference_ptrs_on_map(schema, schema.structs_.get());
      set_reference_ptrs_on_map(schema, schema.enums_.get());
      set_reference_ptrs_on_map(schema, schema.tuples_.get());
      set_reference_ptrs_on_map(schema, schema.unions_.get());
      set_reference_ptrs_on_map(schema, schema.union_helpers_.get());
      schema.root_type_.type_ptr =
          find_in_schema(schema, schema.root_type_.type_name);
    } catch (std::exception& e) {
      return error(e.what());
    }
    return schema;
  }

 private:
  static const schema::Type* find_in_schema(const FlatbufSchema& _schema,
                                            const std::string& _name);

  static void set_reference_ptrs_on_type(const FlatbufSchema& _schema,
                                         Type* _type);

  static void set_reference_ptrs_on_map(
      const FlatbufSchema& _schema, std::map<std::string, schema::Type>* _map);
};

std::ostream& operator<<(std::ostream& _os,
                         const FlatbufSchema& _flatbuf_schema);

}  // namespace rfl::flatbuf::schema

#endif
