#ifndef TOMATO_ASSETREGISTRY_H
#define TOMATO_ASSETREGISTRY_H

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>
#include <memory>
#include "tomato/Logger.h"

namespace tomato
{
    template<typename T>
    class AssetRegistry;

    template<typename T>
    struct AssetHandle
    {
    public:
        ~AssetHandle()
        {
            auto registry = AssetRegistry<T>::GetInstance();

            ++registry.data_.generation;
            registry.freeIndices_.push_back(index);
        }

        T* GetAsset() const
        {
            static auto registry = AssetRegistry<T>::GetInstance();

            if (generation != registry.data_[index].generation)
            {
                TMT_ERR << "Invalid AssetHandle in use. Return nullptr.";
                return nullptr;
            }
            else
                return &registry.data_[index].asset;
        }

    private:
        int generation;
        uint32_t index;
    };

    template<typename T>
    class AssetRegistry
    {
    private:
        AssetRegistry() = default;

        struct Asset
        {
            T asset;
            int generation;
        };

    public:
        ~AssetRegistry();

        AssetRegistry& operator=(const AssetRegistry&) = delete;

        std::shared_ptr<AssetHandle<T>> RegisterAsset(const char* filename);

        static AssetRegistry& GetInstance()
        {
            static AssetRegistry instance;
            return instance;
        }

    private:
        std::vector<Asset> data_;
        std::vector<uint32_t> freeIndices_;

        std::unordered_map<std::string, std::shared_ptr<AssetHandle<T>>> handles_;

        friend struct AssetHandle<T>;
    };
}

#endif //TOMATO_ASSETREGISTRY_H
