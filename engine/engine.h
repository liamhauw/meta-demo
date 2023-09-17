/*
  SPDX license identifier: MIT

  Copyright (C) 2023 Liam Hauw.

  Engine header file.
*/ 

#pragma once

#include <memory>

#include "function/input/input.h"
#include "function/rendering/rendering.h"
#include "function/window/window.h"
#include "function/framework/common/world.h"
#include "resource/asset/asset.h"
#include "resource/config/config.h"

namespace luka {

class Engine {
 public:
  Engine();
  ~Engine();

  Engine(const Engine& engine) = delete;
  Engine(Engine&& engine) = delete;
  Engine& operator=(const Engine& engine) = delete;
  Engine& operator=(Engine&& engine) = delete;

  void Run();

 private:
  // Resource
  std::shared_ptr<Config> config_;
  std::shared_ptr<Asset> asset_;

  // Function
  std::shared_ptr<World> world_;
  std::shared_ptr<Window> window_;
  std::shared_ptr<Input> input_;
  std::shared_ptr<Rendering> rendering_;
};

}  // namespace luka