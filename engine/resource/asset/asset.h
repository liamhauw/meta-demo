#pragma once

#include <fstream>
#include <string>

#include "core/log.h"
#include "core/meta/generated/serializer/all_serializer.h"
#include "core/meta/json.h"

namespace luka {

class Asset {
 public:
  template <typename AssetType>
  bool LoadAsset(const std::string& asset_file, AssetType& out_asset) const {
    std::ifstream asset_json_file{asset_file};
    if (!asset_json_file) {
      THROW("fail to open file");
    }

    std::stringstream buffer;
    buffer << asset_json_file.rdbuf();
    std::string asset_json_text{buffer.str()};

    std::string error;
    auto&& asset_json{Json::parse(asset_json_text, error)};
    if (!error.empty()) {
      THROW("fail to parse json file");
    }

    Serializer::read(asset_json, out_asset);
    return true;
  }
};

}  // namespace luka