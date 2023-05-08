#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace luka
{
template<class T>
inline constexpr const T& Clamp(const T& v, const T& lo, const T& hi) {
  return v < lo ? lo : hi < v ? hi : v;
}

} // namespace luka
