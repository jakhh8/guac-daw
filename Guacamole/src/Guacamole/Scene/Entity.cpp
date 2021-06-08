#include "gmpch.h"
#include "Entity.h"

namespace Guacamole {

    Entity::Entity(entt::entity handle, Scene* scene)
    : m_EntityHandle(handle), m_Scene(scene)
    {
    }
    
}