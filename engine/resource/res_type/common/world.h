
#ifndef LUKA_WORLD_H
#define LUKA_WORLD_H

#include <string>
#include <vector>

class WorldRes {
 public:
  std::string name_;
  std::vector<std::string> level_urls_;
  std::string default_level_url_;
};

#endif  // LUKA_WORLD_H
