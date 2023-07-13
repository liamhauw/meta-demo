#pragma once

#include <string>
#include <mustache.hpp>
namespace Mustache = kainjow::mustache;

class TemplateManager {
 public:
  static TemplateManager* GetInstance() {
    static TemplateManager* instance;
    if (!instance)
      instance = new TemplateManager{};
    return instance;
  }
  void LoadTemplates(const std::string& path, const std::string& template_name);

  std::string RenderByTemplate(const std::string& template_name, Mustache::data& template_data);

 private:
  TemplateManager() {}
  TemplateManager(const TemplateManager&);
  TemplateManager& operator=(const TemplateManager&);
  std::unordered_map<std::string, std::string> template_pool_;
};
