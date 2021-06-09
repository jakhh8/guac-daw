#pragma once

#include <Guacamole.h>
#include "Panels/SceneHierarchyPanel.h"
#include "SoundAPI.h"
#include "Synth.h"

namespace Guacamole {

	class DawLayer : public Layer
	{
	public:
		DawLayer();
		virtual ~DawLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		static double MakeNoise(double time);
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();
	private:
		OrthographicController m_CameraController;

		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
		Entity m_SecondCamera;

		Entity m_HoveredEntity;
		
		bool m_PrimaryCamera = true;
		
		EditorCamera m_EditorCamera;
		
		Ref<Texture2D> m_Texture;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		std::string m_CurrentFile = "";

		int m_GizmoType = -1;
		int m_GizmoMode = 0;

		// Audio/Synth
		SoundAPI::AudioIO m_AudioIO;
		static Synth::Instrument* s_Instrument;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}