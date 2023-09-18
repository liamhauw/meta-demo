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
// #define CLASS(class_name,...) class __attribute__((annotate(#__VA_ARGS__)))
// class_name:public Reflection::object
#else
#define META(...)
#define CLASS(class_name, ...) class class_name
#define STRUCT(struct_name, ...) struct struct_name
#endif  // __REFLECTION_PARSER__

#define REFLECTION_BODY(class_name)                      \
  friend class Reflection::TypeFieldReflectionOparator:: \
      Type##class_name##Operator;                        \
  friend class Serializer;

#define REFLECTION_TYPE(class_name)       \
  namespace Reflection {                  \
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

namespace Reflection {
class TypeMeta;
class FieldAccessor;
class MethodAccessor;
class ArrayAccessor;
class ReflectionInstance;
}  // namespace Reflection

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
typedef std::function<int(Reflection::ReflectionInstance*&, void*)>
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

namespace Reflection {
class TypeMetaRegisterInterface {
 public:
  static void RegisterToClassMap(const char* name, ClassFunctionTuple* value);
  static void RegisterToFieldMap(const char* name, FieldFunctionTuple* value);

  static void RegisterToMethodMap(const char* name, MethodFunctionTuple* value);
  static void RegisterToArrayMap(const char* name, ArrayFunctionTuple* value);

  static void UnregisterAll();
};

class TypeMeta {
  friend class FieldAccessor;
  friend class ArrayAccessor;
  friend class TypeMetaRegisterInterface;

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
  explicit TypeMeta(std::string type_name);

 private:
  std::vector<FieldAccessor, std::allocator<FieldAccessor>> fields_;
  std::vector<MethodAccessor, std::allocator<MethodAccessor>> methods_;
  std::string type_name_;

  bool is_valid_;
};

class FieldAccessor {
  friend class TypeMeta;

 public:
  FieldAccessor();

  void* get(void* instance);
  void set(void* instance, void* value);

  TypeMeta getOwnerTypeMeta();

  bool getTypeMeta(TypeMeta& field_type);
  const char* getFieldName() const;
  const char* getFieldTypeName();
  bool isArrayType();

  FieldAccessor& operator=(const FieldAccessor& dest);

 private:
  FieldAccessor(FieldFunctionTuple* functions);

 private:
  FieldFunctionTuple* m_functions;
  const char* m_field_name;
  const char* m_field_type_name;
};
class MethodAccessor {
  friend class TypeMeta;

 public:
  MethodAccessor();

  void invoke(void* instance);

  const char* getMethodName() const;

  MethodAccessor& operator=(const MethodAccessor& dest);

 private:
  MethodAccessor(MethodFunctionTuple* functions);

 private:
  MethodFunctionTuple* m_functions;
  const char* m_method_name;
};
/**
 *  Function reflection is not implemented, so use this as an std::vector
 * accessor
 */
class ArrayAccessor {
  friend class TypeMeta;

 public:
  ArrayAccessor();
  const char* getArrayTypeName();
  const char* getElementTypeName();
  void set(int index, void* instance, void* element_value);

  void* get(int index, void* instance);
  int getSize(void* instance);

  ArrayAccessor& operator=(ArrayAccessor& dest);

 private:
  ArrayAccessor(ArrayFunctionTuple* array_func);

 private:
  ArrayFunctionTuple* m_func;
  const char* m_array_type_name;
  const char* m_element_type_name;
};

class ReflectionInstance {
 public:
  ReflectionInstance(TypeMeta meta, void* instance)
      : m_meta(meta), m_instance(instance) {}
  ReflectionInstance() : m_meta(), m_instance(nullptr) {}

  ReflectionInstance& operator=(ReflectionInstance& dest);

  ReflectionInstance& operator=(ReflectionInstance&& dest);

 public:
  TypeMeta m_meta;
  void* m_instance;
};

template <typename T>
class ReflectionPtr {
  template <typename U>
  friend class ReflectionPtr;

 public:
  ReflectionPtr(std::string type_name, T* instance)
      : m_type_name(type_name), m_instance(instance) {}
  ReflectionPtr() : m_type_name(), m_instance(nullptr) {}

  ReflectionPtr(const ReflectionPtr& dest)
      : m_type_name(dest.m_type_name), m_instance(dest.m_instance) {}

  template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type */>
  ReflectionPtr<T>& operator=(const ReflectionPtr<U>& dest) {
    if (this == static_cast<void*>(&dest)) {
      return *this;
    }
    m_type_name = dest.m_type_name;
    m_instance = static_cast<T*>(dest.m_instance);
    return *this;
  }

  template<typename U /*, typename = typename std::enable_if<std::is_safely_castable<T*, U*>::value>::type*/>
  ReflectionPtr<T>& operator=(ReflectionPtr<U>&& dest) {
    if (this == static_cast<void*>(&dest)) {
      return *this;
    }
    m_type_name = dest.m_type_name;
    m_instance = static_cast<T*>(dest.m_instance);
    return *this;
  }

  ReflectionPtr<T>& operator=(const ReflectionPtr<T>& dest) {
    if (this == &dest) {
      return *this;
    }
    m_type_name = dest.m_type_name;
    m_instance = dest.m_instance;
    return *this;
  }

  ReflectionPtr<T>& operator=(ReflectionPtr<T>&& dest) {
    if (this == &dest) {
      return *this;
    }
    m_type_name = dest.m_type_name;
    m_instance = dest.m_instance;
    return *this;
  }

  std::string getTypeName() const { return m_type_name; }

  void setTypeName(std::string name) { m_type_name = name; }

  bool operator==(const T* ptr) const { return (m_instance == ptr); }

  bool operator!=(const T* ptr) const { return (m_instance != ptr); }

  bool operator==(const ReflectionPtr<T>& rhs_ptr) const {
    return (m_instance == rhs_ptr.m_instance);
  }

  bool operator!=(const ReflectionPtr<T>& rhs_ptr) const {
    return (m_instance != rhs_ptr.m_instance);
  }

  template <typename T1>
  explicit operator T1*() {
    return static_cast<T1*>(m_instance);
  }

  template <typename T1>
  operator ReflectionPtr<T1>() {
    return ReflectionPtr<T1>(m_type_name, (T1*)(m_instance));
  }

  template <typename T1>
  explicit operator const T1*() const {
    return static_cast<T1*>(m_instance);
  }

  template <typename T1>
  operator const ReflectionPtr<T1>() const {
    return ReflectionPtr<T1>(m_type_name, (T1*)(m_instance));
  }

  T* operator->() { return m_instance; }

  T* operator->() const { return m_instance; }

  T& operator*() { return *(m_instance); }

  T* getPtr() { return m_instance; }

  T* getPtr() const { return m_instance; }

  const T& operator*() const { return *(static_cast<const T*>(m_instance)); }

  T*& getPtrReference() { return m_instance; }

  operator bool() const { return (m_instance != nullptr); }

 private:
  std::string m_type_name{""};
  typedef T m_type;
  T* m_instance{nullptr};
};

}  // namespace Reflection

}  // namespace luka
