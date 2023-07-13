#pragma once
#include "common/schema_module.h"

#include <functional>
#include <string>
#include <filesystem>
#include <mustache.hpp>
#include <utility>

namespace fs = std::filesystem;
namespace Mustache = kainjow::mustache;

namespace Generator {
class GeneratorInterface {
 public:
  GeneratorInterface(std::string reflection_file,
                     std::string generated_path,
                     std::function<std::string(std::string)> get_include_func) :
      reflection_path_(std::move(reflection_file)),
      generated_path_(std::move(generated_path)), get_include_func_(std::move(get_include_func)) {
    if (!fs::exists(reflection_path_)) {
      fs::create_directories(reflection_path_);
    }
  }

  virtual int Generate(std::string path, SchemaMoudle schema) = 0;
  virtual void Finish() {};

  virtual ~GeneratorInterface() = default;

 protected:
  virtual void GenClassRenderData(const std::shared_ptr<Class>& class_temp, Mustache::data& class_defs);
  virtual void GenClassFieldRenderData(const std::shared_ptr<Class>& class_temp, Mustache::data& feild_defs);
  virtual void GenClassMethodRenderData(const std::shared_ptr<Class>& class_temp, Mustache::data& method_defs);

  virtual std::string ProcessFileName(std::string path) = 0;

  std::string reflection_path_;
  std::string generated_path_;
  std::function<std::string(std::string)> get_include_func_;
};
} // namespace Generator
