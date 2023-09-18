/*
  SPDX license identifier: MIT

  Copyright (C) 2023-present Liam Hauw.

  Reflection register source file.
*/

#include "core/meta/reflection/reflection_register.h"

#include "core/json/json.h"
#include "core/meta/generated/reflection/all_reflection.h"
#include "core/meta/generated/serializer/all_serializer.ipp"
#include "core/meta/reflection/reflection.h"
#include "core/meta/serializer/serializer.h"

namespace luka::Reflection {
void TypeMetaRegister::metaUnregister() {
  TypeMetaRegisterInterface::UnregisterAll();
}
}  // namespace luka::Reflection
