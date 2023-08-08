#pragma once

#include "common/namespace.h"
#include "common/precompiled.h"
#include "common/schema_module.h"
#include "cursor/cursor.h"
#include "generator/generator.h"
#include "template_manager/template_manager.h"

class MetaParser {
 public:
  MetaParser(std::string json_header_file, std::string header_file,
             const std::string& generated_path, std::string project_root_path);

  ~MetaParser();

  int Parse();

 private:
  std::string GetIncludeFile(const std::string& name);
  void ParseProject();
  void BuildClassAst(const Cursor& cursor, Namespace& current_namespace);
  void GenerateFiles();

  std::string json_header_file_;
  std::string header_file_;
  std::vector<std::string> generated_path_;
  std::string project_root_path_;

  CXIndex index_{nullptr};
  CXTranslationUnit translation_unit_{nullptr};

  std::vector<Generator::GeneratorInterface*> generators_;

  std::unordered_map<std::string, std::string> type_table_;
  std::unordered_map<std::string, SchemaMoudle> schema_modules_;

  std::vector<const char*> arguments_{
      {"-x", "c++", "-std=c++11", "-D__REFLECTION_PARSER__", "-DNDEBUG",
       "-D__clang__", "-w", "-MG", "-M", "-ferror-limit=0", "-o clangLog.txt"}};
};
