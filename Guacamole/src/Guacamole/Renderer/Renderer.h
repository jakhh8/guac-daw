#pragma once

#include "Guacamole/Renderer/RenderCommand.h"

#include "Guacamole/Renderer/Camera.h"
#include "Guacamole/Renderer/OrthographicCamera.h"
#include "Guacamole/Renderer/Shader.h"

namespace Guacamole {

	class Renderer
	{
	public:
		static void Init();
        static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};

}