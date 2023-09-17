#pragma once

#include "cursor/cursor_type.h"
#include <clang-c/Index.h>
#include <string>
#include <vector>

class Cursor {
 public:
  typedef std::vector<Cursor> List;

  explicit Cursor(const CXCursor& handle);

  CXCursorKind GetKind() const;
  std::string GetSpelling() const;
  std::string GetDisplayName() const;
  std::string GetSourceFile() const;
  bool IsDefinition() const;
  CursorType GetType() const;
  List GetChildren() const;

 private:
  CXCursor cx_cursor_;
};