#pragma once

#include <memory>
#include <string>
#include <vector>

#include "function/framework/common/object_id_allocator.h"
#include "core/meta/reflection/reflection.h"
#include "function/framework/component/component.h"

namespace luka {

class GObject : public std::enable_shared_from_this<GObject> {
 public:
  GObject(GObjectID id) {}

 protected:
  GObjectID id_{kInvlidGObjectID};
  std::string name_;
  std::string definition_url_;

  std::vector<Reflection::ReflectionPtr<Component>> components_;
};

}  // namespace luka