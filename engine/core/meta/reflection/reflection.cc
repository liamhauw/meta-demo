/*
  Copyright (C) 2023-present Liam Hauw.

  SPDX license identifier: MIT.

  Reflection source file.
*/

#include "core/meta/reflection/reflection.h"

#include <cstring>
#include <map>

namespace luka {
namespace reflection {

const char* kUnknownType = "UnknownType";
const char* kUnknown = "Unknown";

static std::map<std::string, ClassFunctionTuple*> g_class_map;
static std::multimap<std::string, FieldFunctionTuple*> g_field_map;
static std::multimap<std::string, MethodFunctionTuple*> g_method_map;
static std::map<std::string, ArrayFunctionTuple*> g_array_map;

void TypeMetaRegisterInterface::RegisterToClassMap(const char* name,
                                                   ClassFunctionTuple* value) {
  if (g_class_map.find(name) == g_class_map.end()) {
    g_class_map.insert(std::make_pair(name, value));
  } else {
    delete value;
  }
}
void TypeMetaRegisterInterface::RegisterToFieldMap(const char* name,
                                                   FieldFunctionTuple* value) {
  g_field_map.insert(std::make_pair(name, value));
}
void TypeMetaRegisterInterface::RegisterToMethodMap(
    const char* name, MethodFunctionTuple* value) {
  g_method_map.insert(std::make_pair(name, value));
}
void TypeMetaRegisterInterface::RegisterToArrayMap(const char* name,
                                                   ArrayFunctionTuple* value) {
  if (g_array_map.find(name) == g_array_map.end()) {
    g_array_map.insert(std::make_pair(name, value));
  } else {
    delete value;
  }
}

void TypeMetaRegisterInterface::UnregisterAll() {
  for (const auto& itr : g_field_map) {
    delete itr.second;
  }
  g_field_map.clear();
  for (const auto& itr : g_class_map) {
    delete itr.second;
  }
  g_class_map.clear();
  for (const auto& itr : g_array_map) {
    delete itr.second;
  }
  g_array_map.clear();
}

TypeMeta::TypeMeta(std::string type_name)
    : type_name_(type_name), is_valid_(false) {
  fields_.clear();
  methods_.clear();

  auto fileds_iter = g_field_map.equal_range(type_name);
  while (fileds_iter.first != fileds_iter.second) {
    FieldAccessor f_field(fileds_iter.first->second);
    fields_.emplace_back(f_field);
    is_valid_ = true;

    ++fileds_iter.first;
  }

  auto methods_iter = g_method_map.equal_range(type_name);
  while (methods_iter.first != methods_iter.second) {
    MethodAccessor f_method(methods_iter.first->second);
    methods_.emplace_back(f_method);
    is_valid_ = true;

    ++methods_iter.first;
  }
}

TypeMeta::TypeMeta() : type_name_(kUnknownType), is_valid_(false) {
  fields_.clear();
  methods_.clear();
}

TypeMeta TypeMeta::NewMetaFromName(std::string type_name) {
  TypeMeta f_type(type_name);
  return f_type;
}

bool TypeMeta::NewArrayAccessorFromName(std::string array_type_name,
                                        ArrayAccessor& accessor) {
  auto iter = g_array_map.find(array_type_name);

  if (iter != g_array_map.end()) {
    ArrayAccessor new_accessor(iter->second);
    accessor = new_accessor;
    return true;
  }

  return false;
}

ReflectionInstance TypeMeta::NewFromNameAndJson(std::string type_name,
                                                const Json& json_context) {
  auto iter = g_class_map.find(type_name);

  if (iter != g_class_map.end()) {
    return ReflectionInstance(TypeMeta(type_name),
                              (std::get<1>(*iter->second)(json_context)));
  }
  return ReflectionInstance();
}

Json TypeMeta::WriteByName(std::string type_name, void* instance) {
  auto iter = g_class_map.find(type_name);

  if (iter != g_class_map.end()) {
    return std::get<2>(*iter->second)(instance);
  }
  return Json();
}

std::string TypeMeta::GetTypeName() { return type_name_; }

int TypeMeta::GetFieldsList(FieldAccessor*& out_list) {
  int count = static_cast<int>(fields_.size());
  out_list = new FieldAccessor[count];
  for (int i = 0; i < count; ++i) {
    out_list[i] = fields_[i];
  }
  return count;
}

int TypeMeta::GetMethodsList(MethodAccessor*& out_list) {
  int count = static_cast<int>(methods_.size());
  out_list = new MethodAccessor[count];
  for (int i = 0; i < count; ++i) {
    out_list[i] = methods_[i];
  }
  return count;
}

int TypeMeta::GetBaseClassReflectionInstanceList(ReflectionInstance*& out_list,
                                                 void* instance) {
  auto iter = g_class_map.find(type_name_);

  if (iter != g_class_map.end()) {
    return (std::get<0>(*iter->second))(out_list, instance);
  }

  return 0;
}

FieldAccessor TypeMeta::GetFieldByName(const char* name) {
  const auto it = std::find_if(
      fields_.begin(), fields_.end(),
      [&](const auto& i) { return std::strcmp(i.GetFieldName(), name) == 0; });
  if (it != fields_.end()) return *it;
  return FieldAccessor(nullptr);
}

MethodAccessor TypeMeta::GetMethodByName(const char* name) {
  const auto it = std::find_if(
      methods_.begin(), methods_.end(),
      [&](const auto& i) { return std::strcmp(i.GetMethodName(), name) == 0; });
  if (it != methods_.end()) return *it;
  return MethodAccessor(nullptr);
}

TypeMeta& TypeMeta::operator=(const TypeMeta& dest) {
  if (this == &dest) {
    return *this;
  }
  fields_.clear();
  fields_ = dest.fields_;

  methods_.clear();
  methods_ = dest.methods_;

  type_name_ = dest.type_name_;
  is_valid_ = dest.is_valid_;

  return *this;
}
FieldAccessor::FieldAccessor()
    : field_type_name_(kUnknownType),
      field_name_(kUnknown),
      functions_(nullptr) {}

FieldAccessor::FieldAccessor(FieldFunctionTuple* functions)
    : functions_(functions),
      field_name_(kUnknown),
      field_type_name_(kUnknownType) {
  if (functions_ == nullptr) {
    return;
  }

  field_type_name_ = (std::get<4>(*functions_))();
  field_name_ = (std::get<3>(*functions_))();
}

void* FieldAccessor::Get(void* instance) {
  return static_cast<void*>((std::get<1>(*functions_))(instance));
}

void FieldAccessor::Set(void* instance, void* value) {
  (std::get<0>(*functions_))(instance, value);
}

TypeMeta FieldAccessor::GetOwnerTypeMeta() {
  TypeMeta f_type((std::get<2>(*functions_))());
  return f_type;
}

bool FieldAccessor::GetTypeMeta(TypeMeta& field_type) {
  TypeMeta f_type(field_type_name_);
  field_type = f_type;
  return f_type.is_valid_;
}

const char* FieldAccessor::GetFieldName() const { return field_name_; }
const char* FieldAccessor::GetFieldTypeName() { return field_type_name_; }

bool FieldAccessor::IsArrayType() { return (std::get<5>(*functions_))(); }

FieldAccessor& FieldAccessor::operator=(const FieldAccessor& dest) {
  if (this == &dest) {
    return *this;
  }
  functions_ = dest.functions_;
  field_name_ = dest.field_name_;
  field_type_name_ = dest.field_type_name_;
  return *this;
}

MethodAccessor::MethodAccessor()
    : functions_(nullptr), method_name_(kUnknown) {}

MethodAccessor::MethodAccessor(MethodFunctionTuple* functions)
    : functions_(functions), method_name_(kUnknown) {
  if (functions_ == nullptr) {
    return;
  }

  method_name_ = (std::get<0>(*functions_))();
}
const char* MethodAccessor::GetMethodName() const {
  return (std::get<0>(*functions_))();
}
MethodAccessor& MethodAccessor::operator=(const MethodAccessor& dest) {
  if (this == &dest) {
    return *this;
  }
  functions_ = dest.functions_;
  method_name_ = dest.method_name_;
  return *this;
}
void MethodAccessor::Invoke(void* instance) {
  (std::get<1>(*functions_))(instance);
}
ArrayAccessor::ArrayAccessor()
    : func_(nullptr),
      array_type_name_("UnKnownType"),
      element_type_name_("UnKnownType") {}

ArrayAccessor::ArrayAccessor(ArrayFunctionTuple* array_func)
    : func_(array_func),
      array_type_name_(kUnknownType),
      element_type_name_(kUnknownType) {
  if (func_ == nullptr) {
    return;
  }

  array_type_name_ = std::get<3>(*func_)();
  element_type_name_ = std::get<4>(*func_)();
}
const char* ArrayAccessor::GetArrayTypeName() { return array_type_name_; }
const char* ArrayAccessor::getElementTypeName() { return element_type_name_; }
void ArrayAccessor::Set(int index, void* instance, void* element_value) {
  std::get<0> (*func_)(index, instance, element_value);
}

void* ArrayAccessor::Get(int index, void* instance) {
  return std::get<1>(*func_)(index, instance);
}

int ArrayAccessor::GetSize(void* instance) {
  return std::get<2>(*func_)(instance);
}

ArrayAccessor& ArrayAccessor::operator=(const ArrayAccessor& dest) {
  if (this == &dest) {
    return *this;
  }
  func_ = dest.func_;
  array_type_name_ = dest.array_type_name_;
  element_type_name_ = dest.element_type_name_;
  return *this;
}

ReflectionInstance& ReflectionInstance::operator=(const ReflectionInstance& dest) {
  if (this == &dest) {
    return *this;
  }
  instance_ = dest.instance_;
  meta_ = dest.meta_;

  return *this;
}

ReflectionInstance& ReflectionInstance::operator=(ReflectionInstance&& dest) {
  if (this == &dest) {
    return *this;
  }
  instance_ = dest.instance_;
  meta_ = dest.meta_;

  return *this;
}
}  // namespace reflection
}  // namespace luka