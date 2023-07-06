#include "common/precompiled.h"
#include "parser/parser.h"

void Parse(int argc, char* argv[]) {
  if (argc != 7) {
    throw std::runtime_error{"argument count error"};
  }

  std::string json_header_file{argv[1]};
  std::string header_file{argv[2]};
  std::string generated_path{argv[3]};
  std::string system_include_path{argv[4]};
  std::string module_name{argv[5]};
  std::string show_error{argv[6]};

  bool is_show_error = show_error != "0";

  MetaParser meta_parser{json_header_file,    header_file, generated_path,
                         system_include_path, module_name, is_show_error};
  meta_parser.parse();
  meta_parser.generateFiles();
}

int main(int argc, char* argv[]) {
  try {
    Parse(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  return 0;
}
