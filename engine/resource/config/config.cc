#include "resource/config/config.h"

#include <fstream>

namespace luka {

Config::Config() {
  std::ifstream config_file{config_file_.generic_string()};
  std::string line;
  while (std::getline(config_file, line)) {
    size_t seperate_pos{line.find_first_of('=')};
    if (seperate_pos > 0 && seperate_pos < (line.length() - 1)) {
      std::string name{line.substr(0, seperate_pos)};
      std::string value{line.substr(seperate_pos + 1, line.length() - seperate_pos - 1)};

      if (name == "world") {
        world_file_ = asset_path_ / "world" / value;
      }
    }
  }
}

const std::filesystem::path& Config::GetWorldFile() const { return world_file_; }

}  // namespace luka
