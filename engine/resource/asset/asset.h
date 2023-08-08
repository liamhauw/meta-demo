#pragma once

#include <fstream>
#include <string>

#include "core/meta/generated/serializer/all_serializer.h"
#include "core/meta/json.h"
#include "core/util/log.h"

namespace luka {

class Asset {
 public:
  template <typename AssetType>
  void LoadAsset(const std::string& asset_file_path, AssetType& asset) const {
    std::ifstream asset_file{asset_file_path};
    if (!asset_file) {
      THROW("fail to open file");
    }

    std::stringstream buffer;
    buffer << asset_file.rdbuf();
    std::string asset_json_text{buffer.str()};

    std::string error;
    auto&& asset_json{Json::parse(asset_json_text, error)};
    if (!error.empty()) {
      THROW("fail to parse json file");
    }

    Serializer::read(asset_json, asset);
  }
};

}  // namespace luka