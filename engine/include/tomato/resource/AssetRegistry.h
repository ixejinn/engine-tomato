#ifndef TOMATO_ASSETREGISTRY_H
#define TOMATO_ASSETREGISTRY_H

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include "tomato/Logger.h"
#include "tomato/resource/tomato_resource.h"

namespace tomato
{
    template<typename T>
    class AssetRegistry
    {
    private:
        AssetRegistry() = default;

    public:
        ~AssetRegistry() = default;

        AssetRegistry& operator=(const AssetRegistry&) = delete;

        static AssetRegistry& GetInstance()
        {
            static AssetRegistry instance;
            return instance;
        }

        template<typename U = T, typename... Args>
        T* Get(const char* filename, Args&&... args);
        T* Get(AssetId id);

        void Clear()
        {
            data_.clear();
            idToIdx_.clear();
        }

    private:
        template<typename U = T, typename... Args>
        void Load(AssetId id, const char* filename, Args&&... args);

        std::vector<std::unique_ptr<T>> data_;
        std::unordered_map<AssetId, uint32_t> idToIdx_;
    };

    template<typename T>
    template<typename U, typename... Args>
    T* AssetRegistry<T>::Get(const char* filename, Args&&... args)
    {
        const auto id = GetAssetID(filename);
        auto it = idToIdx_.find(id);

        if (it == idToIdx_.end())
            Load<U>(id, filename, std::forward<Args>(args)...);

        return data_[it->second].get();
    }

    template<typename T>
    T* AssetRegistry<T>::Get(const AssetId id)
    {
        auto it = idToIdx_.find(id);
        if (it == idToIdx_.end())
            TMT_ERR << "Invalid asset ID: " << id;

        return data_[it->second].get();
    }

    template<typename T>
    template<typename U, typename... Args>
    void AssetRegistry<T>::Load(AssetId id, const char* filename, Args&&... args)
    {
        idToIdx_[id] = data_.size();
        data_.emplace_back(std::make_unique<U>(filename, std::forward<Args>(args)...));
    }
}

#endif //TOMATO_TOMATO_RESOURCE_H
