#pragma once
#include "precompiled.h"

class Class;
class Enum;

struct SchemaMoudle
{
    std::string name;
    std::vector<std::shared_ptr<Class>> classes;
};