#pragma once

#include <string>
#include <vector>

namespace luka {

extern std::string root_directory;

std::vector<char> ReadFile(const std::string& filename);

};