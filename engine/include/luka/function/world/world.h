#pragma once

#include <memory>
#include <string>

#include "luka/resource/config/config.h"

namespace luka {

class World {
 public:
  explicit World(const std::shared_ptr<Config>& config);

 private:
  bool is_world_loaded_{false};
  std::string world_file_;
};

}  // namespace luka