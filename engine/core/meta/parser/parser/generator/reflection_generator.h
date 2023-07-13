#pragma once
#include "generator/generator.h"

namespace Generator {
class ReflectionGenerator : public GeneratorInterface {
 public:
  ReflectionGenerator() = delete;
  ReflectionGenerator(const std::string& generated_path,
                      std::function<std::string(std::string)> get_include_function);
  ~ReflectionGenerator() override = default;

  int Generate(std::string path, SchemaMoudle schema) override;
  void Finish() override;

 protected:
  std::string ProcessFileName(std::string path) override;

 private:
  std::vector<std::string> m_head_file_list;
  std::vector<std::string> m_sourcefile_list;
};
} // namespace Generator
