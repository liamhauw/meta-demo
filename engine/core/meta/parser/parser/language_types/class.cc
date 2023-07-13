#include "common/precompiled.h"

#include "class.h"

BaseClass::BaseClass(const Cursor& cursor) : name(Utils::getTypeNameWithoutNamespace(cursor.GetType())) {}

Class::Class(const Cursor& cursor, const Namespace& current_namespace) :
    TypeInfo(cursor, current_namespace), m_name(cursor.GetDisplayName()),
    m_qualified_name(Utils::getTypeNameWithoutNamespace(cursor.GetType())),
    display_name_(Utils::getNameWithoutFirstM(m_qualified_name))
{
    Utils::replaceAll(m_name, " ", "");
    Utils::replaceAll(m_name, "luka::", "");

    for (auto& child : cursor.GetChildren())
    {
        switch (child.GetKind())
        {
            case CXCursor_CXXBaseSpecifier: {
                auto base_class = new BaseClass(child);

                base_classes_.emplace_back(base_class);
            }
            break;
            // field
            case CXCursor_FieldDecl:
                fields_.emplace_back(new Field(child, current_namespace, this));
                break;
            // method
            case CXCursor_CXXMethod:
                m_methods.emplace_back(new Method(child, current_namespace, this));
            default:
                break;
        }
    }
}

bool Class::ShouldCompile(void) const { return shouldCompileFields()|| shouldCompileMethods(); }

bool Class::shouldCompileFields(void) const
{
    return m_meta_data.getFlag(NativeProperty::All) || m_meta_data.getFlag(NativeProperty::Fields) ||
           m_meta_data.getFlag(NativeProperty::WhiteListFields);
}

bool Class::shouldCompileMethods(void) const{
    
    return m_meta_data.getFlag(NativeProperty::All) || m_meta_data.getFlag(NativeProperty::Methods) ||
           m_meta_data.getFlag(NativeProperty::WhiteListMethods);
}

std::string Class::GetClassName(void) { return m_name; }

bool Class::isAccessible(void) const { return m_enabled; }