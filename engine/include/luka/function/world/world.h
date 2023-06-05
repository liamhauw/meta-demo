#pragma once

#include <string>

namespace luka {

class World {
 public:
  World();

 private:
  bool  is_world_loaded_{false};
  std::string world_file_;
};

}