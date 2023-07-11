#pragma once

#include <clang-c/Index.h>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "meta/meta_data_config.h"
#include "meta/meta_utils.h"
#include "mustache.hpp"

namespace fs = std::filesystem;
namespace Mustache = kainjow::mustache;
