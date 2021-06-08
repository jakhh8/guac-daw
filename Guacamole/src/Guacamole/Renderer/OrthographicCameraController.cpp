#include "gmpch.h"
#include "OrthographicCameraController.h"

#include "Guacamole/Core/Input.h"
#include "Guacamole/Core/KeyCodes.h"

namespace Guacamole {

	OrthographicController::OrthographicController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}
	
	void OrthographicController::OnUpdate(Timestep ts)
	{
		GM_PROFILE_FUNCTION();
	
		if(Input::IsKeyPressed(Key::A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		if(Input::IsKeyPressed(Key::D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
	
		if(Input::IsKeyPressed(Key::W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		if(Input::IsKeyPressed(Key::S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		
		if(m_Rotation)
		{
			if(Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if(Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel * 1.5f;
	}

	void OrthographicController::OnEvent(Event& e)
	{
		GM_PROFILE_FUNCTION();
	
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GM_BIND_EVENT_FN(OrthographicController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GM_BIND_EVENT_FN(OrthographicController::OnWindowResize));
	}

	void OrthographicController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		GM_PROFILE_FUNCTION();
	
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		return false;
	}

	bool OrthographicController::OnWindowResize(WindowResizeEvent& e)
	{
		GM_PROFILE_FUNCTION();
	
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		
		return false;
	}
	
}