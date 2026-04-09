#ifndef TOMATO_TRANSFORM_H
#define TOMATO_TRANSFORM_H

#include "tomato/tomato_math.h"
#include "tomato/Logger.h"

namespace tomato
{
    struct TransformComponent
    {
    public:
        TransformComponent() = default;
        TransformComponent(const Vector3& pos, const Vector3& eulerRot = Vector3{0.f}, const Vector3& scl = Vector3{1.f})
        : position(pos), eulerDegree(eulerRot), scale(scl) {}

        const Vector3& GetPosition() const { return position; }
        void SetPosition(const Vector3& newPos) { position = newPos; }
        void SetPosition(const float x, const float y, const float z) { position = Vector3{x, y, z}; }

        const Vector3& GetEulerDegree() const { return eulerDegree; }
        void SetEulerDegree(const Vector3& newRot)
        {
            eulerDegree = newRot;
            rotDirty = true;
        }
        void SetEulerDegree(const float x, const float y, const float z)
        {
            eulerDegree = Vector3{x, y, z};
            rotDirty = true;
        }

        const Quaternion& GetQuaternion() const
        {
            if (rotDirty)
                TMT_WARN << "Quaternion is dirty. Update required.";

            return rotation;
        }

        const Vector3& GetScale() const { return scale; }
        void SetScale(const Vector3& newScl) { scale = newScl; }
        void SetScale(const float x, const float y, const float z) { scale = Vector3{x, y, z}; }

        const Matrix4& GetTransformMatrix() const { return transformMatrix; }

    private:
        Vector3 position{0.f};
        Vector3 eulerDegree{0.f};
        Vector3 scale{1.f};

        Quaternion rotation;
        bool rotDirty{true};

        /// Local to World.
        Matrix4 transformMatrix;

        friend class TransformSystem;
    };

//    struct PositionComponent { Vector3 position{ 0.f, 0.f, 0.f }; };
//
//    struct RotationComponent {
//        Vector3 eulerDegree;
//        //Quaternion rotation;
//    };
//
//    struct ScaleComponent { Vector3 scale{1.f, 1.f, 1.f}; };
//
//    // Local to World
//    struct WorldMatrixComponent { Matrix4 matrix; };
}

#endif //TOMATO_TRANSFORM_H
