#include "DawLayer.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

#include <Guacamole/Scene/SceneSerializer.h>

#include <Guacamole/Utils/PlatformUtils.h>

#include <Guacamole/Math/Math.h>

namespace Guacamole {

	DawLayer::DawLayer()
		: Layer("Daw"), m_SequencerPanel(s_Sequencer)
	{
	}

	Synth::Sequencer DawLayer::s_Sequencer = Synth::Sequencer();
	
	double DawLayer::MakeNoise(double time)
	{
		// Master volume
		return s_Sequencer.GetTracks()->at(0).GetInstrument()->Sound(time) * 0.1;
	}

	void DawLayer::OnAttach()
	{
		GM_PROFILE_FUNCTION();
		
		m_AudioIO.SetUserFunc(Guacamole::DawLayer::MakeNoise);
		s_Sequencer.GetTracks()->push_back(Synth::Track(new Synth::Harmonica()));
	}

	void DawLayer::OnDetach()
	{
		GM_PROFILE_FUNCTION();

		m_AudioIO.Stop();
	}

	void DawLayer::OnUpdate(Timestep ts)
	{
		GM_PROFILE_FUNCTION();
		
		KeyCode keys[16] = { Key::Z, Key::S, Key::X, Key::C, Key::F, Key::V, Key::G, Key::B, Key::N, Key::J, Key::M, Key::K, Key::Comma, Key::L, Key::Period, Key::Slash };

		auto instrument = s_Sequencer.GetTracks()->at(0).GetInstrument();
		
		for(int i = 0; i < 16; i++)
		{
			if(Input::IsKeyPressed(keys[i]) && !instrument->Notes.at(i).IsNoteOn)
				instrument->Notes.at(i).NoteOn(m_AudioIO.GetTime());
			else if(!Input::IsKeyPressed(keys[i]) && instrument->Notes.at(i).IsNoteOn)
				instrument->Notes.at(i).NoteOff(m_AudioIO.GetTime());
		}
	}

	void DawLayer::OnImGuiRender()
	{
		GM_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if(opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if(opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSize;

		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();
				
				if(ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();
				
				if(ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene();
				
				if(ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
					SaveSceneAs();
			
				if(ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_SequencerPanel.OnImGuiRender();

		ImGui::Begin("Stats");
		//ImGui::Text(" Instrument: %s", s_Instrument->Tag.c_str());
		//ImGui::Text("\tVolume: %f", s_Instrument->Volume);
		
		// Stats
		ImGui::End();

		// Dockspace demo
		ImGui::End();
	}

	void DawLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(GM_BIND_EVENT_FN(DawLayer::OnKeyPressed));
	}
	
	bool DawLayer::OnKeyPressed(KeyPressedEvent& e) 
	{
		if(e.GetRepeatCount() > 0)
			return false;

		return true;
	}
	
	void DawLayer::NewScene() 
	{
		s_Sequencer.GetTracks()->clear();
		m_CurrentFile = "";
	}
	
	void DawLayer::OpenScene() 
	{
		std::optional<std::string> filepath = FileDialogs::OpenFile("Guacamole Scene (*.guac)\0*.guac\0");
		if(filepath)
		{
			//SceneSerializer serializer(m_ActiveScene);
			//serializer.Deserialize(*filepath);
			// Get instruments + set vector TODO
			m_CurrentFile = *filepath;
		}
	}
	
	void DawLayer::SaveScene() 
	{
		if(!m_CurrentFile.empty())
		{
			//SceneSerializer serializer(m_ActiveScene);
			//serializer.Serialize(m_CurrentFile);
		}
	}
	
	void DawLayer::SaveSceneAs() 
	{
		std::optional<std::string> filepath = FileDialogs::SaveFile("Guacamole Scene (*.guac)\0*.guac\0");
		if(filepath)
		{
			//SceneSerializer serializer(m_ActiveScene);
			//serializer.Serialize(*filepath);
			m_CurrentFile = *filepath;
		}
	}

}