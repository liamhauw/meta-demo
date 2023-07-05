#include "common/precompiled.h"
#include "parser/parser.h"

void parse(std::string project_input_file_name, std::string source_include_file_name,
           const std::string& include_path, std::string sys_include, const std::string& module_name,
           const std::string& show_errors) {
  std::cout << std::endl;
  std::cout << "Parsing meta data for target \"" << module_name << "\"" << std::endl;
  std::fstream input_file;

  bool is_show_errors = "0" != show_errors;

  MetaParser parser(project_input_file_name, source_include_file_name, include_path, sys_include,
                    module_name, is_show_errors);

  std::cout << "Parsing in " << include_path << std::endl;
  int result = parser.parse();
  if (0 != result) {
    return;
  }

  parser.generateFiles();
}

int main(int, char* argv[]) {
  if (argv[1] != nullptr && argv[2] != nullptr && argv[3] != nullptr && argv[4] != nullptr &&
      argv[5] != nullptr && argv[6] != nullptr) {
    MetaParser::prepare();

    parse(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

  } else {
    std::cerr
        << "Arguments parse error!" << std::endl
        << "Please call the tool like this:" << std::endl
        << "meta_parser  project_file_name  include_file_name_to_generate  project_base_directory "
           "sys_include_directory module_name showErrors(0 or 1)"
        << std::endl
        << std::endl;
    return -1;
  }

  return 0;
}
