#pragma once

#include <string>

namespace luka {

class Asset {
 public:
  template <typename AssetType>
  bool LoadAsset(const std::string& asset_file, AssetType& out_asset) const {}
};

}  // namespace luka