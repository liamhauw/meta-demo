#pragma once

#include "type_info.h"

#include "field.h"
#include "method.h"

struct BaseClass
{
    BaseClass(const Cursor& cursor);

    std::string name;
};

class Class : public TypeInfo
{
    // to access m_qualifiedName
    friend class Field;
    friend class Method;
    friend class Parser;

public:
    Class(const Cursor& cursor, const Namespace& current_namespace);

    virtual bool ShouldCompile(void) const;

    bool shouldCompileFields(void) const;
    bool shouldCompileMethods(void) const;

    template<typename T>
    using SharedPtrVector = std::vector<std::shared_ptr<T>>;

    std::string GetClassName(void);

    SharedPtrVector<BaseClass> base_classes_;

public:
    std::string m_name;

    std::string m_qualified_name;

    SharedPtrVector<Field> fields_;
    SharedPtrVector<Method> m_methods;

    std::string display_name_;

    bool isAccessible(void) const;
};
