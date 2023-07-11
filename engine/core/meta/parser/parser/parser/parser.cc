#include "parser.h"

#include <utility>

#include "common/precompiled.h"
#include "generator/reflection_generator.h"
#include "generator/serializer_generator.h"
#include "language_types/class.h"

MetaParser::MetaParser(std::string json_header_file, std::string header_file,
                       const std::string& generated_path, std::string system_include_path,
                       std::string module_name, const std::string& show_error, std::string project_root_path)
    : json_header_file_{std::move(json_header_file)},
      header_file_{std::move(header_file)},
      system_include_path{std::move(system_include_path)},
      module_name_{std::move(module_name)},
      project_root_path_{project_root_path} {
  generated_path_ = Utils::Split(generated_path, ";");
  show_error_ = show_error != "0";

  generators_.emplace_back(new Generator::SerializerGenerator(
      generated_path_[0],
      [this](auto&& ph1) { return GetIncludeFile(std::forward<decltype(ph1)>(ph1)); }));
  generators_.emplace_back(new Generator::ReflectionGenerator(
      generated_path_[0],
      [this](auto&& ph1) { return GetIncludeFile(std::forward<decltype(ph1)>(ph1)); }));
}

MetaParser::~MetaParser() {
  for (auto item : generators_) {
    delete item;
  }
  generators_.clear();

  if (translation_unit_) clang_disposeTranslationUnit(translation_unit_);
  if (index_) clang_disposeIndex(index_);
}

std::string MetaParser::GetIncludeFile(const std::string& name) {
  auto iter{type_table_.find(name)};
  return iter == type_table_.end() ? std::string{} : iter->second;
}

int MetaParser::Parse() {
  ParseProject();

  int show_error{show_error_ ? 1 : 0};
  index_ = clang_createIndex(true, show_error);

  std::string pre_include = "-I";
  std::string sys_include_temp;
  if (!(system_include_path == "*")) {
    sys_include_temp = pre_include + system_include_path;
    arguments.emplace_back(sys_include_temp.c_str());
  }

  auto paths{generated_path_};
  for (auto& path : paths) {
    path = pre_include + path;
    arguments.emplace_back(path.c_str());
  }

  fs::path header_file(header_file_);
  if (!fs::exists(header_file)) {
    throw std::runtime_error{header_file.string() + " is not exist"};
  }

  translation_unit_ = clang_createTranslationUnitFromSourceFile(index_, header_file_.c_str(),
                                                                static_cast<int>(arguments.size()),
                                                                arguments.data(), 0, nullptr);
  const Cursor cursor{clang_getTranslationUnitCursor(translation_unit_)};

  Namespace temp_namespace;

  BuildClassAst(cursor, temp_namespace);

  temp_namespace.clear();

  GenerateFiles();

  return 0;
}

void MetaParser::ParseProject() {
  std::fstream json_header_fs(json_header_file_, std::ios::in);
  if (json_header_fs.fail()) {
    throw std::runtime_error{"fail to open " + json_header_file_};
  }

  std::stringstream buffer;
  buffer << json_header_fs.rdbuf();
  std::string context{buffer.str()};
  auto inlcude_files{Utils::Split(context, ";")};

  std::fstream header_fs;
  header_fs.open(header_file_, std::ios::out);
  if (!header_fs.is_open()) {
    throw std::runtime_error{"fail to open " + header_file_};
  }

  std::string output_filename{Utils::GetFileName(header_file_)};

  Utils::Replace(output_filename, ".", "_");
  Utils::Replace(output_filename, " ", "_");
  Utils::ToUpper(output_filename);

  header_fs << "#ifndef __" << output_filename << "__" << std::endl;
  header_fs << "#define __" << output_filename << "__" << std::endl;

  for (const auto& include_item : inlcude_files) {
    std::string temp_string(include_item);
    temp_string = project_root_path_ + "\\" + temp_string;
    Utils::Replace(temp_string, '\\', '/');
    if (temp_string.back() == '\n') {
      temp_string.pop_back();
    }
    header_fs << "#include  \"" << temp_string << "\"" << std::endl;
  }

  header_fs << "#endif" << std::endl;
  header_fs.close();
}

void MetaParser::BuildClassAst(const Cursor& cursor, Namespace& current_namespace) {
  auto cursor_children{cursor.GetChildren()};
  for (auto& child : cursor_children) {
    auto kind = child.GetKind();

    auto class_ptr = std::make_shared<Class>(child, current_namespace);
    if (class_ptr->m_name == "Matrix4") {
      int i = 1;
    }

    // actual definition and a class or struct
    if (child.IsDefinition() && (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl)) {

      if (class_ptr->ShouldCompile()) {
        auto file{class_ptr->GetSourceFile()};
        schema_modules_[file].classes.emplace_back(class_ptr);
        type_table_[class_ptr->display_name_] = file;
      }
    } else {
      if (kind == CXCursor_Namespace) {
        auto display_name{child.GetDisplayName()};
        if (!display_name.empty()) {
          current_namespace.emplace_back(display_name);
          BuildClassAst(child, current_namespace);
          current_namespace.pop_back();
        }
      }
    }
  }
}

void MetaParser::GenerateFiles() {
  std::cout << "schemas module size : " << schema_modules_.size() << std::endl;
  for (auto& schema : schema_modules_) {
    for (auto& generator_iter : generators_) {
      generator_iter->generate(schema.first, schema.second);
    }
  }

  for (auto generator_iter : generators_) {
    generator_iter->finish();
  }
}


