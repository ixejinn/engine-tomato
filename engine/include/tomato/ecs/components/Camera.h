#ifndef TOMATO_CAMERA_H
#define TOMATO_CAMERA_H

#include "tomato/tomato_math.h"

namespace tomato
{
    enum ProjectionMode
    {
        PERSPECTIVE,
        ORTHOGONAL
    };

	struct CameraComponent
	{
        ProjectionMode mode{PERSPECTIVE};

        float degree{45.f};
        float zNear{0.1f};
        float zFar{100.f};
        Matrix4 viewProjection{1.f};
	};
}


#endif