#pragma once

#include "type_info.h"

class Class;

class Field : public TypeInfo
{

public:
    Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent = nullptr);

    virtual ~Field(void) {}

    bool ShouldCompile(void) const;

public:
    bool m_is_const;

    Class* m_parent;

    std::string name_;
    std::string display_name_;
    std::string type_;

    std::string m_default;

    bool isAccessible(void) const;
};