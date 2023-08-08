#include "common/precompiled.h"
#include "parser/parser.h"

int main(int argc, char* argv[]) {
  try {
    if (argc != 6) {
      throw std::runtime_error{"argument count error"};
    }
    MetaParser meta_parser{argv[1], argv[2], argv[3], argv[4], argv[5]};
    meta_parser.Parse();

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  return 0;
}
