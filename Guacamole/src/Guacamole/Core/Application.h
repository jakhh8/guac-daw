#pragma once

#include "Guacamole/Core/Base.h"

#include "Guacamole/Core/Window.h"
#include "Guacamole/Core/LayerStack.h"
#include "Guacamole/Events/Event.h"
#include "Guacamole/Events/ApplicationEvent.h"

#include "Guacamole/Core/Timestep.h"

#include "Guacamole/ImGui/ImGuiLayer.h"

namespace Guacamole {

	class Application {
	public:
		Application(const std::string& name = "Guacamole app");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		
		static inline Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		static Application* s_Instance;
	
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}