#pragma once

#include <string>
#include <clang-c/Index.h>

class Cursor;

class CursorType {
 public:
  CursorType(const CXType& handle);

  [[nodiscard]] std::string GetDisplayName() const;

  [[nodiscard]] int GetArgumentCount() const;

  [[nodiscard]] CursorType GetArgument(unsigned index) const;

  [[nodiscard]] CursorType GetCanonicalType() const;

  [[nodiscard]] Cursor GetDeclaration() const;

  [[nodiscard]] CXTypeKind GetKind() const;

  [[nodiscard]] bool IsConst() const;

 private:
  CXType handle_;
};