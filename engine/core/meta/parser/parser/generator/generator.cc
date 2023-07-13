#include "generator/generator.h"



#include "language_types/class.h"

namespace Generator {

void GeneratorInterface::GenClassRenderData(const std::shared_ptr<Class>& class_temp, Mustache::data& class_defs) {
  class_defs.set("class_name", class_temp->GetClassName());
  class_defs.set("class_base_class_size", std::to_string(class_temp->base_classes_.size()));
  class_defs.set("class_need_register", true);

  if (!class_temp->base_classes_.empty()) {
    Mustache::data class_base_class_defines{Mustache::data::type::list};
    class_defs.set("class_has_base", true);
    for (int index = 0; index < class_temp->base_classes_.size(); ++index) {
      Mustache::data class_base_class_def;
      class_base_class_def.set("class_base_class_name", class_temp->base_classes_[index]->name);
      class_base_class_def.set("class_base_class_index", std::to_string(index));
      class_base_class_defines.push_back(class_base_class_def);
    }
    class_defs.set("class_base_class_defines", class_base_class_defines);
  }

  Mustache::data class_field_defines{Mustache::data::type::list};
  GenClassFieldRenderData(class_temp, class_field_defines);
  class_defs.set("class_field_defines", class_field_defines);

  Mustache::data class_method_defines{Mustache::data::type::list};
  GenClassMethodRenderData(class_temp, class_method_defines);
  class_defs.set("class_method_defines", class_method_defines);
}

void GeneratorInterface::GenClassFieldRenderData(const std::shared_ptr<Class>& class_temp, Mustache::data& feild_defs) {
  static const std::string vector_prefix{"std::vector<"};

  for (auto& field : class_temp->fields_) {
    if (!field->ShouldCompile()) {
      continue;
    }
    Mustache::data filed_define;

    filed_define.set("class_field_name", field->name_);
    filed_define.set("class_field_type", field->type_);
    filed_define.set("class_field_display_name", field->display_name_);
    bool is_vector{field->type_.find(vector_prefix) == 0};
    filed_define.set("class_field_is_vector", is_vector);
    feild_defs.push_back(filed_define);
  }
}

void GeneratorInterface::GenClassMethodRenderData(const std::shared_ptr<Class>& class_temp,
                                                  Mustache::data& method_defs) {
  for (auto& method : class_temp->m_methods) {
    if (!method->ShouldCompile()) {
      continue;
    }
    Mustache::data method_define;

    method_define.set("class_method_name", method->m_name);
    method_defs.push_back(method_define);
  }
}
} // namespace Generator
