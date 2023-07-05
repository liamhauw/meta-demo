#pragma once

#include <string>

#include "core/math/vector3.h"
#include "core/meta/reflection/reflection.h"

namespace luka {
REFLECTION_TYPE(LevelRes)
CLASS(LevelRes, Fields) {
  REFLECTION_BODY(LevelRes);

 public:
  std::string character_name_;
};

}  // namespace luka
