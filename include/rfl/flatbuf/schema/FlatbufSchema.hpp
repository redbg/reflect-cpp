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
  Ref<std::map<std::string, schema::Type>> maps_;
  Ref<std::map<std::string, schema::Type>> unions_;
  Ref<std::map<std::string, schema::Type>> union_helpers_;

  Type::Reference root_type_;

  Result<FlatbufSchema> set_reference_ptrs() const;

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
