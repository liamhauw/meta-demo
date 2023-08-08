
#ifndef LUKA_WORLD_H
#define LUKA_WORLD_H

#include <string>
#include <vector>

#include "core/meta/reflection/reflection.h"

namespace luka {

REFLECTION_TYPE(WorldRes)
CLASS(WorldRes, Fields) {
  REFLECTION_BODY(WorldRes);

 public:
  // world name
  std::string m_name;

  // all level urls for this world
  // std::vector<std::string> m_level_urls;

  // the default level for this world, which should be first loading level
  std::string m_default_level_url;
};

}



#endif  // LUKA_WORLD_H
