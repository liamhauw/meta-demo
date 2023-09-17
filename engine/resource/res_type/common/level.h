/*
  SPDX license identifier: MIT

  Copyright (C) 2023-present Liam Hauw.

  Level header file.
*/

#pragma once

#include <string>

#include "core/meta/reflection/reflection.h"

namespace luka {
REFLECTION_TYPE(LevelRes)
CLASS(LevelRes, Fields) {
  REFLECTION_BODY(LevelRes);

 public:
  std::string character_name_;
};

}  // namespace luka
