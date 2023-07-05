#pragma once

#include "core/meta/reflection/reflection.h"

namespace luka {
class GObject;

REFLECTION_TYPE(Component)
CLASS(Component, WhiteListFields) {
  REFLECTION_BODY(Component)
 public:
  Component() = default;
  virtual ~Component() = default;

 protected:
  std::weak_ptr<GObject> parent_object_;
  bool is_dirty_{false};
  bool is_scale_dirty_{false};
};

}  // namespace luka