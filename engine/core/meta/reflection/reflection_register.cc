#include <assert.h>

#include "core/meta/json.h"
#include "core/meta/reflection/reflection.h"
#include "core/meta/reflection/reflection_register.h"
#include "core/meta/serializer/serializer.h"

#include "../generated/_generated/reflection/all_reflection.h"
#include "../generated/_generated/serializer/all_serializer.ipp"

namespace luka
{
    namespace Reflection
    {
        void TypeMetaRegister::metaUnregister() { TypeMetaRegisterinterface::unregisterAll(); }
    } // namespace Reflection
} // namespace luka
