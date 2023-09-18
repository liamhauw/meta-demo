/*
  Copyright (C) 2023-present Liam Hauw.

  SPDX license identifier: MIT.

  Reflection header file.
*/

#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "core/json/json.h"

namespace luka {

#if defined(__REFLECTION_PARSER__)
#define META(...) __attribute__((annotate(#__VA_ARGS__)))
#define CLASS(class_name, ...) \
  class __attribute__((annotate(#__VA_ARGS__))) class_name
#define STRUCT(struct_name, ...) \
  struct __attribute__((annotate(#__VA_ARGS__))) struct_name
#else
#define META(...)
#define CLASS(class_name, ...) class class_name
#define STRUCT(struct_name, ...) struct struct_name
#endif  // __REFLECTION_PARSER__

#define REFLECTION_BODY(class_name)                      \
  friend class reflection::TypeFieldReflectionOparator:: \
      Type##class_name##Operator;                        \
  friend class Serializer;

#define REFLECTION_TYPE(class_name)       \
  namespace reflection {                  \
  namespace TypeFieldReflectionOparator { \
  class Type##class_name##Operator;       \
  }                                       \
  };

#define REGISTER_FIELD_TO_MAP(name, value) \
  TypeMetaRegisterInterface::RegisterToFieldMap(name, value);
#define REGISTER_METHOD_TO_MAP(name, value) \
  TypeMetaRegisterinterface::RegisterToMethodMap(name, value);
#define REGISTER_BASE_CLASS_TO_MAP(name, value) \
  TypeMetaRegisterInterface::RegisterToClassMap(name, value);
#define REGISTER_ARRAY_TO_MAP(name, value) \
  TypeMetaRegisterInterface::RegisterToArrayMap(name, value);
#define UNREGISTER_ALL TypeMetaRegisterinterface::UnregisterAll();

namespace reflection {

class TypeMeta;
class FieldAccessor;
class MethodAccessor;
class ArrayAccessor;
class ReflectionInstance;

}  // namespace reflection

typedef std::function<void(void*, void*)> SetFuncion;
typedef std::function<void*(void*)> GetFuncion;
typedef std::function<const char*()> GetNameFuncion;
typedef std::function<void(int, void*, void*)> SetArrayFunc;
typedef std::function<void*(int, void*)> GetArrayFunc;
typedef std::function<int(void*)> GetSizeFunc;
typedef std::function<bool()> GetBoolFunc;
typedef std::function<void(void*)> InvokeFunction;
typedef std::function<void*(const Json&)> ConstructorWithJson;
typedef std::function<Json(void*)> WriteJsonByName;
typedef std::function<int(reflection::ReflectionInstance*&, void*)>
    GetBaseClassReflectionInstanceListFunc;

typedef std::tuple<SetFuncion, GetFuncion, GetNameFuncion, GetNameFuncion,
                   GetNameFuncion, GetBoolFunc>
    FieldFunctionTuple;
typedef std::tuple<GetNameFuncion, InvokeFunction> MethodFunctionTuple;
typedef std::tuple<GetBaseClassReflectionInstanceListFunc, ConstructorWithJson,
                   WriteJsonByName>
    ClassFunctionTuple;
typedef std::tuple<SetArrayFunc, GetArrayFunc, GetSizeFunc, GetNameFuncion,
                   GetNameFuncion>
    ArrayFunctionTuple;

namespace reflection {
class TypeMetaRegisterInterface {
 public:
  static void RegisterToClassMap(const char* name, ClassFunctionTuple* value);
  static void RegisterToFieldMap(const char* name, FieldFunctionTuple* value);
  static void RegisterToMethodMap(const char* name, MethodFunctionTuple* value);
  static void RegisterToArrayMap(const char* name, ArrayFunctionTuple* value);

  static void UnregisterAll();
};

class TypeMeta {
 public:
  TypeMeta();
  static TypeMeta NewMetaFromName(std::string type_name);
  static bool NewArrayAccessorFromName(std::string array_type_name,
                                       ArrayAccessor& accessor);
  static ReflectionInstance NewFromNameAndJson(std::string type_name,
                                               const Json& json_context);
  static Json WriteByName(std::string type_name, void* instance);
  std::string GetTypeName();
  int GetFieldsList(FieldAccessor*& out_list);
  int GetMethodsList(MethodAccessor*& out_list);
  int GetBaseClassReflectionInstanceList(ReflectionInstance*& out_list,
                                         void* instance);
  FieldAccessor GetFieldByName(const char* name);
  MethodAccessor GetMethodByName(const char* name);
  bool IsValid() const { return is_valid_; }
  TypeMeta& operator=(const TypeMeta& dest);

 private:
  friend class FieldAccessor;
  friend class ArrayAccessor;
  friend class TypeMetaRegisterInterface;

  explicit TypeMeta(std::string type_name);

  std::vector<FieldAccessor, std::allocator<FieldAccessor>> fields_;
  std::vector<MethodAccessor, std::allocator<MethodAccessor>> methods_;
  std::string type_name_;
  bool is_valid_;
};

class FieldAccessor {
 public:
  FieldAccessor();
  void* Get(void* instance);
  void Set(void* instance, void* value);
  TypeMeta GetOwnerTypeMeta();
  bool GetTypeMeta(TypeMeta& field_type);
  const char* GetFieldName() const;
  const char* GetFieldTypeName();
  bool IsArrayType();
  FieldAccessor& operator=(const FieldAccessor& dest);

 private:
  friend class TypeMeta;

  FieldAccessor(FieldFunctionTuple* functions);

  FieldFunctionTuple* functions_;
  const char* field_name_;
  const char* field_type_name_;
};

class MethodAccessor {
 public:
  MethodAccessor();
  void Invoke(void* instance);
  const char* GetMethodName() const;
  MethodAccessor& operator=(const MethodAccessor& dest);

 private:
  friend class TypeMeta;

  MethodAccessor(MethodFunctionTuple* functions);

  MethodFunctionTuple* functions_;
  const char* method_name_;
};

class ArrayAccessor {
 public:
  ArrayAccessor();
  const char* GetArrayTypeName();
  const char* getElementTypeName();
  void Set(int index, void* instance, void* element_value);
  void* Get(int index, void* instance);
  int GetSize(void* instance);

  ArrayAccessor& operator=(ArrayAccessor& dest);

 private:
  friend class TypeMeta;

  ArrayAccessor(ArrayFunctionTuple* array_func);

  ArrayFunctionTuple* func_;
  const char* array_type_name_;
  const char* element_type_name_;
};

class ReflectionInstance {
 public:
  ReflectionInstance(TypeMeta meta, void* instance)
      : meta_(meta), instance_(instance) {}
  ReflectionInstance() : meta_(), instance_(nullptr) {}
  ReflectionInstance& operator=(ReflectionInstance& dest);
  ReflectionInstance& operator=(ReflectionInstance&& dest);

 public:
  TypeMeta meta_;
  void* instance_;
};

template <typename T>
class ReflectionPtr {
 public:
  ReflectionPtr(std::string type_name, T* instance)
      : type_name_(type_name), instance_(instance) {}
  ReflectionPtr() : type_name_(), instance_(nullptr) {}

  ReflectionPtr(const ReflectionPtr& dest)
      : type_name_(dest.type_name_), instance_(dest.instance_) {}

  template <typename U>
  ReflectionPtr<T>& operator=(const ReflectionPtr<U>& dest) {
    if (this == static_cast<void*>(&dest)) {
      return *this;
    }
    type_name_ = dest.type_name_;
    instance_ = static_cast<T*>(dest.instance_);
    return *this;
  }

  template <typename U>
  ReflectionPtr<T>& operator=(ReflectionPtr<U>&& dest) {
    if (this == static_cast<void*>(&dest)) {
      return *this;
    }
    type_name_ = dest.type_name_;
    instance_ = static_cast<T*>(dest.instance_);
    return *this;
  }

  ReflectionPtr<T>& operator=(const ReflectionPtr<T>& dest) {
    if (this == &dest) {
      return *this;
    }
    type_name_ = dest.type_name_;
    instance_ = dest.instance_;
    return *this;
  }

  ReflectionPtr<T>& operator=(ReflectionPtr<T>&& dest) {
    if (this == &dest) {
      return *this;
    }
    type_name_ = dest.type_name_;
    instance_ = dest.instance_;
    return *this;
  }

  std::string GetTypeName() const { return type_name_; }

  void SetTypeName(std::string name) { type_name_ = name; }

  bool operator==(const T* ptr) const { return (instance_ == ptr); }

  bool operator!=(const T* ptr) const { return (instance_ != ptr); }

  bool operator==(const ReflectionPtr<T>& rhs_ptr) const {
    return (instance_ == rhs_ptr.instance_);
  }

  bool operator!=(const ReflectionPtr<T>& rhs_ptr) const {
    return (instance_ != rhs_ptr.instance_);
  }

  template <typename T1>
  explicit operator T1*() {
    return static_cast<T1*>(instance_);
  }

  template <typename T1>
  operator ReflectionPtr<T1>() {
    return ReflectionPtr<T1>(type_name_, (T1*)(instance_));
  }

  template <typename T1>
  explicit operator const T1*() const {
    return static_cast<T1*>(instance_);
  }

  template <typename T1>
  operator const ReflectionPtr<T1>() const {
    return ReflectionPtr<T1>(type_name_, (T1*)(instance_));
  }

  T* operator->() { return instance_; }

  T* operator->() const { return instance_; }

  T& operator*() { return *(instance_); }

  T* getPtr() { return instance_; }

  T* getPtr() const { return instance_; }

  const T& operator*() const { return *(static_cast<const T*>(instance_)); }

  T*& GetPtrReference() { return instance_; }

  operator bool() const { return (instance_ != nullptr); }

 private:
  template <typename U>
  friend class ReflectionPtr;

  std::string type_name_{""};
  T* instance_{nullptr};
};

}  // namespace reflection

}  // namespace luka
