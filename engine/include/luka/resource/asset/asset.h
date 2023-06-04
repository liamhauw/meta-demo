#pragma once

#include <string>
#include <vector>

namespace luka {

class Asset {
 public:
  static std::vector<char> ReadFile(const std::string &filename);
};

}  // namespace luka