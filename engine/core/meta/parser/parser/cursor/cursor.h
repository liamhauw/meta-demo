#pragma once

#include "cursor_type.h"

class Cursor {
 public:
  typedef std::vector<Cursor> List;

  typedef CXCursorVisitor Visitor;

  explicit Cursor(const CXCursor& handle);

  [[nodiscard]] CXCursorKind GetKind() const;
  [[nodiscard]] std::string GetSpelling() const;
  [[nodiscard]] std::string GetDisplayName() const;
  [[nodiscard]] std::string GetSourceFile() const;
  [[nodiscard]] bool IsDefinition() const;
  [[nodiscard]] CursorType GetType() const;
  [[nodiscard]] List GetChildren() const;

 private:
  CXCursor cx_cursor_;
};