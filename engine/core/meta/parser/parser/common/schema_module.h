#pragma once

#include <vector>
#include <memory>
#include <string>

class Class;

struct SchemaMoudle {
  std::string name;
  std::vector<std::shared_ptr<Class>> classes;
};