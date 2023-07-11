#include "common/precompiled.h"

#include "cursor.h"
#include "cursor_type.h"

CursorType::CursorType(const CXType& handle) : handle_(handle) {}

std::string CursorType::GetDisplayName() const {
  std::string display_name;

  Utils::ToString(clang_getTypeSpelling(handle_), display_name);

  return display_name;
}

int CursorType::GetArgumentCount() const { return clang_getNumArgTypes(handle_); }

CursorType CursorType::GetArgument(unsigned index) const { return clang_getArgType(handle_, index); }

CursorType CursorType::GetCanonicalType() const { return clang_getCanonicalType(handle_); }

Cursor CursorType::GetDeclaration() const { return Cursor{clang_getTypeDeclaration(handle_)}; }

CXTypeKind CursorType::GetKind() const { return handle_.kind; }

bool CursorType::IsConst() const { return clang_isConstQualifiedType(handle_) != 0; }