#pragma once

#include <glm/glm.hpp>

namespace Apostle {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		inline const glm::mat4& GetProjection() const { return m_Projection; }
	
	protected:
		glm::mat4 m_Projection;
	};

}