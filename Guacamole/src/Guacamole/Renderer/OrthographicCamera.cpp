#include "gmpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Guacamole {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_Projection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		GM_PROFILE_FUNCTION();
	
		m_ViewProjectionMatrix =  m_Projection * m_ViewMatrix;
	}

	void OrthographicCamera::CalculateViewMatrix()
	{
		GM_PROFILE_FUNCTION();
	
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		GM_PROFILE_FUNCTION();
	
		m_Projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix =  m_Projection * m_ViewMatrix;
	}
	
}