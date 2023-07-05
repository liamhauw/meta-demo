#pragma once

#include <cstddef>
#include <limits>

namespace luka {

using GObjectID = std::size_t;

constexpr GObjectID kInvlidGObjectID{std::numeric_limits<std::size_t>::max()};

}
