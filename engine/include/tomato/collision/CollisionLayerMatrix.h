#ifndef TOMATO_COLLISIONLAYERMATRIX_H
#define TOMATO_COLLISIONLAYERMATRIX_H

#include "tomato/containers/EnumArray.h"
#include "tomato/collision/CollisionConstants.h"
#include "tomato/utils/BitmaskOperators.h"

namespace tomato
{
    struct CollisionLayerMatrix
    {
    public:
        CollisionLayerMatrix()
        {
            Initialize();
        }

        void SetCollisionLayer(CollisionLayer layer1, CollisionLayer layer2, bool enabled)
        {
            if (enabled)
            {
                matrix_[layer1] |= layer2;
                matrix_[layer2] |= layer1;
            }
            else
            {
                matrix_[layer1] &= ~layer2;
                matrix_[layer2] &= ~layer1;
            }
        }

        bool CanCollide(CollisionLayer layer1, CollisionLayer layer2)
        {
            return HasFlag(matrix_[layer1], layer2);
        }

    private:
        void Initialize()
        {
            matrix_[CollisionLayer::Default] |= CollisionLayer::Default;
        }

        EnumArray<CollisionLayer, CollisionLayer> matrix_;
    };
}

#endif //TOMATO_COLLISIONLAYERMATRIX_H
