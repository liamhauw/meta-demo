#include <assert.h>

#include "core/json/json.h"
#include "core/meta/reflection/reflection.h"
#include "core/meta/reflection/reflection_register.h"
#include "core/meta/serializer/serializer.h"

#include "../generated/reflection/all_reflection.h"
#include "../generated/serializer/all_serializer.ipp"

namespace luka::Reflection {
void TypeMetaRegister::metaUnregister() { TypeMetaRegisterInterface::UnregisterAll(); }
} // namespace luka
