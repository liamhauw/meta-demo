/*
  SPDX license identifier: MIT

  Copyright (C) 2023-present Liam Hauw.

  World header file.
*/

#pragma once

#include <string>
#include <vector>

#include "core/meta/reflection/reflection.h"

namespace luka {

REFLECTION_TYPE(WorldRes)
CLASS(WorldRes, Fields) {
  REFLECTION_BODY(WorldRes);

 public:
  // world name
  std::string name_;

  // all level urls for this world
   std::vector<std::string> level_urls_;

  // the default level for this world, which should be first loading level
  std::string default_level_url_;
};

}

