#include "DawLayer.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

#include <Guacamole/Scene/SceneSerializer.h>

#include <Guacamole/Utils/PlatformUtils.h>

#include <Guacamole/Math/Math.h>

namespace Guacamole {

	DawLayer::DawLayer()
		: Layer("Daw"), m_CameraController(1280.0f / 720.0f)
	{
	}

	struct Bell : public Synth::Instrument
	{
		Bell()
		{
			for(int i = 0; i < 16; i++)
			{
				Synth::NoteADSR note;
				
				note.AttackTime = 0.01;
				note.DecayTime = 1.0;
				note.SustainAmplitude = 0.0;
				note.ReleaseTime = 1.0;

				note.Note = i;

				Notes.push_back(note);
			}

			Volume = 1.0;
		}

		virtual double Sound(double time) override
		{
			double output = 0.0;

			for(auto note : Notes)
			{
				output += note.GetAmplitude(time) *
				(
					  1.0 * Synth::osc(Synth::calcFrequency(note.Note), time, OSC_SINE, 5.0, 0.001)
					+ 0.5 * Synth::osc(Synth::calcFrequency(note.Note) * 2.0, time, OSC_SINE)
					+ 0.25 * Synth::osc(Synth::calcFrequency(note.Note) * 3.0, time, OSC_SINE)
				);
			}

			return output * Volume;
		}
	};

	struct Harmonica : public Synth::Instrument
	{
		Harmonica()
		{
			for(int i = 0; i < 16; i++)
			{
				Synth::NoteADSR note;
				
				note.AttackTime = 0.05;
				note.DecayTime = 1.0;
				note.SustainAmplitude = 0.95;
				note.ReleaseTime = 0.1;

				note.Note = i;

				Notes.push_back(note);
			}

			Volume = 1.0;
		}

		virtual double Sound(double time) override
		{
			double output = 0.0;
			
			for(auto note : Notes)
			{
				output += note.GetAmplitude(time) *
				(
					  1.0 * Synth::osc(Synth::calcFrequency(note.Note), time, OSC_SQUARE, 5.0, 0.001)
					+ 0.5 * Synth::osc(Synth::calcFrequency(note.Note) * 1.5, time, OSC_SQUARE)
					+ 0.25 * Synth::osc(Synth::calcFrequency(note.Note) * 2.0, time, OSC_SQUARE)
					+ 0.05 * Synth::osc(0, time, OSC_NOISE)
				);
			}

			return output * Volume;
		}
	};

	struct Piano : public Synth::Instrument
	{
		Piano()
		{
			for(int i = 0; i < 16; i++)
			{
				Synth::NoteADSR note;
				
				note.AttackTime = 0.05;
				note.SustainAmplitude = 0.95;

				note.Note = i;

				Notes.push_back(note);
			}

			Volume = 1.0;
		}

		virtual double Sound(double time) override
		{
			double output = 0.0;
			
			for(auto note : Notes)
			{
				output += note.GetAmplitude(time) *
				(
					  1.0 * Synth::osc(Synth::calcFrequency(note.Note) * 0.5, time, OSC_SINE)
					+ 0.5 * Synth::osc(Synth::calcFrequency(note.Note), time, OSC_SAW_ANA)
				);
			}

			return output * Volume;
		}
	};

	Synth::Instrument* DawLayer::s_Instrument = new Harmonica();
	
	double DawLayer::MakeNoise(double time)
	{
		// Master volume
		return s_Instrument->Sound(time) * 0.1;
	}

	void DawLayer::OnAttach()
	{
		GM_PROFILE_FUNCTION();
		
		m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_AudioIO.SetUserFunc(Guacamole::DawLayer::MakeNoise);
#if 0
		// Entity
		auto square = m_ActiveScene->CreateEntity("Green square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		auto redSquare = m_ActiveScene->CreateEntity("Red square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
			}
			
			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				
				float speed = 5.0f;

				if(Input::IsKeyPressed(Key::A))
					translation.x -= speed * ts;
				if(Input::IsKeyPressed(Key::D))
					translation.x += speed * ts;
				if(Input::IsKeyPressed(Key::W))
					translation.y += speed * ts;
				if(Input::IsKeyPressed(Key::S))
					translation.y -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		
	}

	void DawLayer::OnDetach()
	{
		GM_PROFILE_FUNCTION();

		m_AudioIO.Stop();
	}

	void DawLayer::OnUpdate(Timestep ts)
	{
		GM_PROFILE_FUNCTION();

		// Resize
		if (Guacamole::FramebufferSpecification spec = m_Framebuffer->GetSpecification(); m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
		}
		
		// Update
		if(m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
		}
		
		m_EditorCamera.OnUpdate(ts);
		
		KeyCode keys[16] = { Key::Z, Key::S, Key::X, Key::C, Key::F, Key::V, Key::G, Key::B, Key::N, Key::J, Key::M, Key::K, Key::Comma, Key::L, Key::Period, Key::Slash };
		
		for(int i = 0; i < 16; i++)
		{
			if(Input::IsKeyPressed(keys[i]) && !s_Instrument->Notes.at(i).IsNoteOn)
				s_Instrument->Notes.at(i).NoteOn(m_AudioIO.GetTime());
			else if(!Input::IsKeyPressed(keys[i]) && s_Instrument->Notes.at(i).IsNoteOn)
				s_Instrument->Notes.at(i).NoteOff(m_AudioIO.GetTime());
		}
		
		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		RenderCommand::Clear();

		// Clear object id pass
		m_Framebuffer->ClearAttachment(1, -1);

		// Update scene
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto[mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		my = viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if(mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		m_Framebuffer->Unbind();
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

		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		std::string name = "None";
		if(m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;

		ImGui::Text("Hovered entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();

		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Quad count: %d", stats.QuadCount);
		ImGui::Text("Vertex count: %d", stats.GetTotalVertexCount());
		ImGui::Text("Index count: %d", stats.GetTotalIndexCount());
		
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if(selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera
			// Runtime camera
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation and scale
			if(m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 15.0f; // Snap to 15.0 degrees for rotation
			
			float snapValues[3] = { snapValue, snapValue, snapValue };
			
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, (ImGuizmo::MODE)m_GizmoMode,
				glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if(ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation =  rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}
		
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void DawLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(GM_BIND_EVENT_FN(DawLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(GM_BIND_EVENT_FN(DawLayer::OnMouseButtonPressed));
	}
	
	bool DawLayer::OnKeyPressed(KeyPressedEvent& e) 
	{
		if(e.GetRepeatCount() > 0)
			return false;

		return true;
	}
	
	bool DawLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) 
	{
		if(e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if(m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}

		return false;
	}
	
	void DawLayer::NewScene() 
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_CurrentFile = "";
	}
	
	void DawLayer::OpenScene() 
	{
		std::optional<std::string> filepath = FileDialogs::OpenFile("Guacamole Scene (*.guac)\0*.guac\0");
		if(filepath)
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(*filepath);
			m_CurrentFile = *filepath;
		}
	}
	
	void DawLayer::SaveScene() 
	{
		if(!m_CurrentFile.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(m_CurrentFile);
		}
	}
	
	void DawLayer::SaveSceneAs() 
	{
		std::optional<std::string> filepath = FileDialogs::SaveFile("Guacamole Scene (*.guac)\0*.guac\0");
		if(filepath)
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(*filepath);
			m_CurrentFile = *filepath;
		}
	}

}