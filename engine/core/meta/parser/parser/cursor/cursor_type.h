#pragma once

#include <string>
#include <clang-c/Index.h>

class Cursor;

class CursorType {
 public:
  CursorType(const CXType& handle);

  std::string GetDisplayName() const;

  int GetArgumentCount() const;

  CursorType GetArgument(unsigned index) const;

  CursorType GetCanonicalType() const;

  Cursor GetDeclaration() const;

  CXTypeKind GetKind() const;

  bool IsConst() const;

 private:
  CXType handle_;
};