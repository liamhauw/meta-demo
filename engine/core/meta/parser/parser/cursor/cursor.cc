
#include "cursor.h"
#include "meta/meta_utils.h"

Cursor::Cursor(const CXCursor& handle) : cx_cursor_(handle) {}

CXCursorKind Cursor::GetKind() const { return cx_cursor_.kind; }

std::string Cursor::GetSpelling() const {
  std::string spelling;

  Utils::ToString(clang_getCursorSpelling(cx_cursor_), spelling);

  return spelling;
}

std::string Cursor::GetDisplayName() const {
  std::string display_name;

  Utils::ToString(clang_getCursorDisplayName(cx_cursor_), display_name);

  return display_name;
}

std::string Cursor::GetSourceFile() const {
  auto range{clang_Cursor_getSpellingNameRange(cx_cursor_, 0, 0)};

  auto start{clang_getRangeStart(range)};

  CXFile file;
  unsigned line, column, offset;

  clang_getFileLocation(start, &file, &line, &column, &offset);

  std::string filename;

  Utils::ToString(clang_getFileName(file), filename);

  return filename;
}

bool Cursor::IsDefinition() const { return clang_isCursorDefinition(cx_cursor_); }

CursorType Cursor::GetType() const { return CursorType{clang_getCursorType(cx_cursor_)}; }

Cursor::List Cursor::GetChildren() const {
  List children;

  auto visitor{[](CXCursor cursor, CXCursor parent, CXClientData data) {
    auto container = static_cast<List*>(data);

    container->emplace_back(cursor);

    if (cursor.kind == CXCursor_LastPreprocessing)
      return CXChildVisit_Break;

    return CXChildVisit_Continue;
  }};

  clang_visitChildren(cx_cursor_, visitor, &children);

  return children;
}
