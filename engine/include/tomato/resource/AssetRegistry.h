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

        template<typename... Args>
        T* Get(const char* filename, Args&&... args);
        T* Get(const char* filename);
        T* Get(AssetId id);

        void Clear()
        {
            data_.clear();
            idToIdx_.clear();
        }

    private:
        template<typename... Args>
        void Load(const char* filename, Args&&... args);
        void Load(const char* filename);

        std::vector<std::unique_ptr<T>> data_;
        std::unordered_map<AssetId, uint32_t> idToIdx_;
    };

    template<typename T>
    template<typename... Args>
    T* AssetRegistry<T>::Get(const char* filename, Args&&... args)
    {
        const auto id = GetAssetID(filename);
        if (idToIdx_.find(id) == idToIdx_.end())
            Load(filename, args...);

        return data_[idToIdx_[GetAssetID(filename)]].get();
    }

    template<typename T>
    T* AssetRegistry<T>::Get(const char* filename)
    {
        const auto id = GetAssetID(filename);
        if (idToIdx_.find(id) == idToIdx_.end())
            Load(filename);

        return data_[idToIdx_[id]].get();
    }

    template<typename T>
    T* AssetRegistry<T>::Get(const AssetId id)
    {
        if (idToIdx_.find(id) == idToIdx_.end())
            TMT_ERR << "Invalid asset ID: " << id;

        return data_[idToIdx_[id]].get();
    }

    template<typename T>
    template<typename... Args>
    void AssetRegistry<T>::Load(const char* filename, Args&&... args)
    {
        idToIdx_[GetAssetID(filename)] = data_.size();
        data_.emplace_back(args...);
    }

    template<typename T>
    void AssetRegistry<T>::Load(const char* filename)
    {
        idToIdx_[GetAssetID(filename)] = data_.size();
        data_.emplace_back(filename);
    }
}

#endif //TOMATO_TOMATO_RESOURCE_H
