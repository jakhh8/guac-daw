#pragma once

#include "Guacamole/Scene/Scene.h"

#include "Guacamole/Core/Base.h"

namespace Guacamole
{

    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void Serialize(const std::string& filepath);
        void SerializeRuntime(const std::string& filepath);

        bool Deserialize(const std::string& filepath);
        bool DeserializeRuntime(const std::string& filepath);
    private:
        Ref<Scene> m_Scene;
    };
    
}