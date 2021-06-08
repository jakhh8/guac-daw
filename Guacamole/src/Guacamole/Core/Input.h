#pragma once

#include <glm/glm.hpp>

#include "Guacamole/Core/KeyCodes.h"
#include "Guacamole/Core/MouseCodes.h"

namespace Guacamole {

	class Input
	{
    public:
        static bool IsKeyPressed(const KeyCode keycode);
        
        static bool IsMouseButtonPressed(const MouseCode button);
        static glm::vec2 GetMousePos();
        static float GetMouseX();
        static float GetMouseY();
	};
	
}