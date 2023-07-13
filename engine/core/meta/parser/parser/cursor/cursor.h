#pragma once

#include "cursor/cursor_type.h"
#include <clang-c/Index.h>
#include <string>
#include <vector>

class Cursor {
 public:
  typedef std::vector<Cursor> List;

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