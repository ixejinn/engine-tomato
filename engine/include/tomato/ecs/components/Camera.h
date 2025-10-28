#ifndef TOMATO_CAMERA_H
#define TOMATO_CAMERA_H

#include <glm/glm.hpp>

namespace tomato
{
	struct CameraComponent
	{
		glm::mat4 view{ 1.0f };
		glm::mat4 projection{ 1.0f };
		//float fov{ 45.0f };
		//float nearClip{ 0.1f };
		//float farClip{ 100.0f };
		bool active = true;
	};
}


#endif