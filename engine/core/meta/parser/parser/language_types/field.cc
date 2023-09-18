#include "common/precompiled.h"

#include "class.h"
#include "field.h"

Field::Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent) :
    TypeInfo(cursor, current_namespace), m_is_const(cursor.GetType().IsConst()), m_parent(parent),
    name_(cursor.GetSpelling()), display_name_(Utils::getNameWithoutLast(name_)),
    type_(Utils::getTypeNameWithoutNamespace(cursor.GetType()))
{
    Utils::replaceAll(type_, " ", "");
    Utils::replaceAll(type_, "luka::", "");

    auto ret_string = Utils::getStringWithoutQuot(m_meta_data.getProperty("default"));
    m_default       = ret_string;
}

bool Field::ShouldCompile(void) const { return isAccessible(); }

bool Field::isAccessible(void) const
{
    return ((m_parent->m_meta_data.getFlag(NativeProperty::Fields) ||
             m_parent->m_meta_data.getFlag(NativeProperty::All)) &&
            !m_meta_data.getFlag(NativeProperty::Disable)) ||
           (m_parent->m_meta_data.getFlag(NativeProperty::WhiteListFields) &&
            m_meta_data.getFlag(NativeProperty::Enable));
}
