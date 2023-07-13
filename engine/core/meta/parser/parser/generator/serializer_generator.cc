#include "generator/serializer_generator.h"
#include "common/precompiled.h"
#include "language_types/class.h"

namespace Generator
{
    SerializerGenerator::SerializerGenerator(std::string                             source_directory,
                                             std::function<std::string(std::string)> get_include_function) :
        GeneratorInterface(source_directory + "/serializer", source_directory, get_include_function)
    {
      TemplateManager::GetInstance()->LoadTemplates(generated_path_, "all_serializer.h");
      TemplateManager::GetInstance()->LoadTemplates(generated_path_, "all_serializer.ipp");
      TemplateManager::GetInstance()->LoadTemplates(generated_path_, "common_serializer_file");
    }


    std::string SerializerGenerator::ProcessFileName(std::string path)
    {
        auto relativeDir = fs::path(path).filename().replace_extension("serializer.gen.h").string();
        return reflection_path_ + "/" + relativeDir;
    }
    int SerializerGenerator::Generate(std::string path, SchemaMoudle schema)
    {
        std::string file_path = ProcessFileName(path);

        Mustache::data muatache_data;
        Mustache::data include_headfiles(Mustache::data::type::list);
        Mustache::data class_defines(Mustache::data::type::list);

        include_headfiles.push_back(
            Mustache::data("headfile_name", Utils::makeRelativePath(generated_path_, path).string()));
        for (auto class_temp : schema.classes)
        {
            if (!class_temp->shouldCompileFields())
                continue;

            Mustache::data class_def;
          GenClassRenderData(class_temp, class_def);

            // deal base class
            for (int index = 0; index < class_temp->base_classes_.size(); ++index)
            {
                auto include_file = get_include_func_(class_temp->base_classes_[index]->name);
                if (!include_file.empty())
                {
                    auto include_file_base = ProcessFileName(include_file);
                    if (file_path != include_file_base)
                    {
                        include_headfiles.push_back(Mustache::data(
                            "headfile_name", Utils::makeRelativePath(generated_path_, include_file_base).string()));
                    }
                }
            }
            for (auto field : class_temp->fields_)
            {
                if (!field->ShouldCompile())
                    continue;
                // deal vector
                if (field->type_.find("std::vector") == 0)
                {
                    auto include_file = get_include_func_(field->name_);
                    if (!include_file.empty())
                    {
                        auto include_file_base = ProcessFileName(include_file);
                        if (file_path != include_file_base)
                        {
                            include_headfiles.push_back(Mustache::data(
                                "headfile_name", Utils::makeRelativePath(generated_path_, include_file_base).string()));
                        }
                    }
                }
                // deal normal
            }
            class_defines.push_back(class_def);
            m_class_defines.push_back(class_def);
        }

        muatache_data.set("class_defines", class_defines);
        muatache_data.set("include_headfiles", include_headfiles);
        std::string render_string =
            TemplateManager::GetInstance()->RenderByTemplate("common_serializer_file", muatache_data);
        Utils::saveFile(render_string, file_path);

        m_include_headfiles.push_back(
            Mustache::data("headfile_name", Utils::makeRelativePath(generated_path_, file_path).string()));
        return 0;
    }

    void SerializerGenerator::Finish()
    {
        Mustache::data mustache_data;
        mustache_data.set("class_defines", m_class_defines);
        mustache_data.set("include_headfiles", m_include_headfiles);

        std::string render_string = TemplateManager::GetInstance()->RenderByTemplate("all_serializer.h", mustache_data);
        Utils::saveFile(render_string, reflection_path_ + "/all_serializer.h");
        render_string = TemplateManager::GetInstance()->RenderByTemplate("all_serializer.ipp", mustache_data);
        Utils::saveFile(render_string, reflection_path_ + "/all_serializer.ipp");
    }

    SerializerGenerator::~SerializerGenerator() {}
} // namespace Generator
