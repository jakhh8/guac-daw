#pragma once

// For use by Guacamole applications

#include "Guacamole/Core/Base.h"

#include "Guacamole/Core/Application.h"
#include "Guacamole/Core/Layer.h"
#include "Guacamole/Core/Log.h"
#include "Guacamole/Core/Assert.h"

#include "Guacamole/Core/Timestep.h"

// ---------------------Input---------------------
#include "Guacamole/Core/Input.h"
#include "Guacamole/Core/KeyCodes.h"
#include "Guacamole/Core/MouseCodes.h"

// ---------------------ImGui---------------------
#include "Guacamole/ImGui/ImGuiLayer.h"

// ---------------------Scene---------------------
#include "Guacamole/Scene/Scene.h"
#include "Guacamole/Scene/Entity.h"
#include "Guacamole/Scene/ScriptableEntity.h"
#include "Guacamole/Scene/Components.h"

// -------------------Rendering-------------------
#include "Guacamole/Renderer/Renderer.h"
#include "Guacamole/Renderer/Renderer2D.h"
#include "Guacamole/Renderer/RenderCommand.h"

#include "Guacamole/Renderer/VertexArray.h"
#include "Guacamole/Renderer/Buffer.h"
#include "Guacamole/Renderer/Shader.h"
#include "Guacamole/Renderer/Framebuffer.h"
#include "Guacamole/Renderer/Texture.h"

#include "Guacamole/Renderer/Camera.h"
#include "Guacamole/Renderer/OrthographicCamera.h"
#include "Guacamole/Renderer/OrthographicCameraController.h"