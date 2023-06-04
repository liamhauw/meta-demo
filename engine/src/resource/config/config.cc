#include "luka/resource/config/config.h"

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

      if (name == "model_path") {
        model_path_ = asset_path_ / value;
      } else if (name == "shader_path") {
        shader_path_ = asset_path_ / value;
      } else if (name == "texture_path") {
        texture_path_ = asset_path_ / value;
      }
    }
  }
}

std::filesystem::path Config::model_path() { return model_path_; }
std::filesystem::path Config::shader_path() { return shader_path_; }
std::filesystem::path Config::texture_path() { return texture_path_; }

}  // namespace luka
