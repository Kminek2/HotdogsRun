#pragma once
#include <functional>
#include <memory>
#include <string>
#include <stdexcept>

#include "SceeneScript.h"

class Application;

class Scene
{
private:
    static std::unordered_map<std::string, SceeneScript*> templates;

    static std::shared_ptr<Scene> loadedScene;

    // Create a new scene from a registered template
    static SceeneScript* Load(const std::string& name) {
        if (templates.find(name) != templates.end()) {
            return templates[name];
        }
        throw std::runtime_error("Scene template not found: " + name);
    }

    SceeneScript* sceneScript;

    static void registerTemplate(const std::string& name, SceeneScript* templateFunction) {
        templates[name] = std::move(templateFunction);
    }

public:
    Scene(SceeneScript* sceneScript);

    friend Application;
};

