/*
  Copyright (C) 2023-present Liam Hauw.

  SPDX license identifier: MIT.   

  Reflection source file.
*/

#include "core/meta/reflection/reflection.h"

#include <cstring>
#include <map>

namespace luka {
namespace Reflection {
const char* k_unknown_type = "UnknownType";
const char* k_unknown = "Unknown";

static std::map<std::string, ClassFunctionTuple*> m_class_map;
static std::multimap<std::string, FieldFunctionTuple*> m_field_map;
static std::multimap<std::string, MethodFunctionTuple*> m_method_map;
static std::map<std::string, ArrayFunctionTuple*> m_array_map;

void TypeMetaRegisterInterface::RegisterToFieldMap(const char* name,
                                                   FieldFunctionTuple* value) {
  m_field_map.insert(std::make_pair(name, value));
}
void TypeMetaRegisterInterface::RegisterToMethodMap(
    const char* name, MethodFunctionTuple* value) {
  m_method_map.insert(std::make_pair(name, value));
}
void TypeMetaRegisterInterface::RegisterToArrayMap(const char* name,
                                                   ArrayFunctionTuple* value) {
  if (m_array_map.find(name) == m_array_map.end()) {
    m_array_map.insert(std::make_pair(name, value));
  } else {
    delete value;
  }
}

void TypeMetaRegisterInterface::RegisterToClassMap(const char* name,
                                                   ClassFunctionTuple* value) {
  if (m_class_map.find(name) == m_class_map.end()) {
    m_class_map.insert(std::make_pair(name, value));
  } else {
    delete value;
  }
}

void TypeMetaRegisterInterface::UnregisterAll() {
  for (const auto& itr : m_field_map) {
    delete itr.second;
  }
  m_field_map.clear();
  for (const auto& itr : m_class_map) {
    delete itr.second;
  }
  m_class_map.clear();
  for (const auto& itr : m_array_map) {
    delete itr.second;
  }
  m_array_map.clear();
}

TypeMeta::TypeMeta(std::string type_name) : type_name_(type_name) {
  is_valid_ = false;
  fields_.clear();
  methods_.clear();

  auto fileds_iter = m_field_map.equal_range(type_name);
  while (fileds_iter.first != fileds_iter.second) {
    FieldAccessor f_field(fileds_iter.first->second);
    fields_.emplace_back(f_field);
    is_valid_ = true;

    ++fileds_iter.first;
  }

  auto methods_iter = m_method_map.equal_range(type_name);
  while (methods_iter.first != methods_iter.second) {
    MethodAccessor f_method(methods_iter.first->second);
    methods_.emplace_back(f_method);
    is_valid_ = true;

    ++methods_iter.first;
  }
}

TypeMeta::TypeMeta() : type_name_(k_unknown_type), is_valid_(false) {
  fields_.clear();
  methods_.clear();
}

TypeMeta TypeMeta::NewMetaFromName(std::string type_name) {
  TypeMeta f_type(type_name);
  return f_type;
}

bool TypeMeta::NewArrayAccessorFromName(std::string array_type_name,
                                        ArrayAccessor& accessor) {
  auto iter = m_array_map.find(array_type_name);

  if (iter != m_array_map.end()) {
    ArrayAccessor new_accessor(iter->second);
    accessor = new_accessor;
    return true;
  }

  return false;
}

ReflectionInstance TypeMeta::NewFromNameAndJson(std::string type_name,
                                                const Json& json_context) {
  auto iter = m_class_map.find(type_name);

  if (iter != m_class_map.end()) {
    return ReflectionInstance(TypeMeta(type_name),
                              (std::get<1>(*iter->second)(json_context)));
  }
  return ReflectionInstance();
}

Json TypeMeta::WriteByName(std::string type_name, void* instance) {
  auto iter = m_class_map.find(type_name);

  if (iter != m_class_map.end()) {
    return std::get<2>(*iter->second)(instance);
  }
  return Json();
}

std::string TypeMeta::GetTypeName() { return type_name_; }

int TypeMeta::GetFieldsList(FieldAccessor*& out_list) {
  int count = fields_.size();
  out_list = new FieldAccessor[count];
  for (int i = 0; i < count; ++i) {
    out_list[i] = fields_[i];
  }
  return count;
}

int TypeMeta::GetMethodsList(MethodAccessor*& out_list) {
  int count = methods_.size();
  out_list = new MethodAccessor[count];
  for (int i = 0; i < count; ++i) {
    out_list[i] = methods_[i];
  }
  return count;
}

int TypeMeta::GetBaseClassReflectionInstanceList(ReflectionInstance*& out_list,
                                                 void* instance) {
  auto iter = m_class_map.find(type_name_);

  if (iter != m_class_map.end()) {
    return (std::get<0>(*iter->second))(out_list, instance);
  }

  return 0;
}

FieldAccessor TypeMeta::GetFieldByName(const char* name) {
  const auto it = std::find_if(
      fields_.begin(), fields_.end(),
      [&](const auto& i) { return std::strcmp(i.getFieldName(), name) == 0; });
  if (it != fields_.end()) return *it;
  return FieldAccessor(nullptr);
}

MethodAccessor TypeMeta::GetMethodByName(const char* name) {
  const auto it = std::find_if(
      methods_.begin(), methods_.end(),
      [&](const auto& i) { return std::strcmp(i.getMethodName(), name) == 0; });
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
FieldAccessor::FieldAccessor() {
  m_field_type_name = k_unknown_type;
  m_field_name = k_unknown;
  m_functions = nullptr;
}

FieldAccessor::FieldAccessor(FieldFunctionTuple* functions)
    : m_functions(functions) {
  m_field_type_name = k_unknown_type;
  m_field_name = k_unknown;
  if (m_functions == nullptr) {
    return;
  }

  m_field_type_name = (std::get<4>(*m_functions))();
  m_field_name = (std::get<3>(*m_functions))();
}

void* FieldAccessor::get(void* instance) {
  // todo: should check validation
  return static_cast<void*>((std::get<1>(*m_functions))(instance));
}

void FieldAccessor::set(void* instance, void* value) {
  // todo: should check validation
  (std::get<0>(*m_functions))(instance, value);
}

TypeMeta FieldAccessor::getOwnerTypeMeta() {
  // todo: should check validation
  TypeMeta f_type((std::get<2>(*m_functions))());
  return f_type;
}

bool FieldAccessor::getTypeMeta(TypeMeta& field_type) {
  TypeMeta f_type(m_field_type_name);
  field_type = f_type;
  return f_type.is_valid_;
}

const char* FieldAccessor::getFieldName() const { return m_field_name; }
const char* FieldAccessor::getFieldTypeName() { return m_field_type_name; }

bool FieldAccessor::isArrayType() {
  // todo: should check validation
  return (std::get<5>(*m_functions))();
}

FieldAccessor& FieldAccessor::operator=(const FieldAccessor& dest) {
  if (this == &dest) {
    return *this;
  }
  m_functions = dest.m_functions;
  m_field_name = dest.m_field_name;
  m_field_type_name = dest.m_field_type_name;
  return *this;
}

MethodAccessor::MethodAccessor() {
  m_method_name = k_unknown;
  m_functions = nullptr;
}

MethodAccessor::MethodAccessor(MethodFunctionTuple* functions)
    : m_functions(functions) {
  m_method_name = k_unknown;
  if (m_functions == nullptr) {
    return;
  }

  m_method_name = (std::get<0>(*m_functions))();
}
const char* MethodAccessor::getMethodName() const {
  return (std::get<0>(*m_functions))();
}
MethodAccessor& MethodAccessor::operator=(const MethodAccessor& dest) {
  if (this == &dest) {
    return *this;
  }
  m_functions = dest.m_functions;
  m_method_name = dest.m_method_name;
  return *this;
}
void MethodAccessor::invoke(void* instance) {
  (std::get<1>(*m_functions))(instance);
}
ArrayAccessor::ArrayAccessor()
    : m_func(nullptr),
      m_array_type_name("UnKnownType"),
      m_element_type_name("UnKnownType") {}

ArrayAccessor::ArrayAccessor(ArrayFunctionTuple* array_func)
    : m_func(array_func) {
  m_array_type_name = k_unknown_type;
  m_element_type_name = k_unknown_type;
  if (m_func == nullptr) {
    return;
  }

  m_array_type_name = std::get<3>(*m_func)();
  m_element_type_name = std::get<4>(*m_func)();
}
const char* ArrayAccessor::getArrayTypeName() { return m_array_type_name; }
const char* ArrayAccessor::getElementTypeName() { return m_element_type_name; }
void ArrayAccessor::set(int index, void* instance, void* element_value) {
  // todo: should check validation
  size_t count = getSize(instance);
  // todo: should check validation(index < count)
  std::get<0> (*m_func)(index, instance, element_value);
}

void* ArrayAccessor::get(int index, void* instance) {
  // todo: should check validation
  size_t count = getSize(instance);
  // todo: should check validation(index < count)
  return std::get<1>(*m_func)(index, instance);
}

int ArrayAccessor::getSize(void* instance) {
  // todo: should check validation
  return std::get<2>(*m_func)(instance);
}

ArrayAccessor& ArrayAccessor::operator=(ArrayAccessor& dest) {
  if (this == &dest) {
    return *this;
  }
  m_func = dest.m_func;
  m_array_type_name = dest.m_array_type_name;
  m_element_type_name = dest.m_element_type_name;
  return *this;
}

ReflectionInstance& ReflectionInstance::operator=(ReflectionInstance& dest) {
  if (this == &dest) {
    return *this;
  }
  m_instance = dest.m_instance;
  m_meta = dest.m_meta;

  return *this;
}

ReflectionInstance& ReflectionInstance::operator=(ReflectionInstance&& dest) {
  if (this == &dest) {
    return *this;
  }
  m_instance = dest.m_instance;
  m_meta = dest.m_meta;

  return *this;
}
}  // namespace Reflection
}  // namespace luka