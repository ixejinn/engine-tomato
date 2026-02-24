#ifndef TOMATO_ASSETREGISTRY_H
#define TOMATO_ASSETREGISTRY_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "tomato/resource/tomato_resource.h"
#include "tomato/Logger.h"

namespace tomato
{
    constexpr AssetId FNV1a64B(const char* str)
    {
        uint64_t hash = 14695981039346656037ULL;
        while (*str)
        {
            hash ^= (uint64_t)(*str);
            hash *= 1099511628211ULL;
            ++str;
        }

        return hash;
    }

    constexpr AssetId GetAssetID(const char* name)
    {
        return FNV1a64B(name);
    }

    template<typename T>
    class AssetRegistry
    {
    private:
        AssetRegistry() = default;

    public:
        static AssetRegistry& GetInstance()
        {
            static AssetRegistry instance;
            return instance;
        }

        ~AssetRegistry()
        {
            Clear();
        }

        AssetRegistry(const AssetRegistry&) = delete;
        AssetRegistry& operator=(const AssetRegistry&) = delete;

        void Register(const char* name, std::unique_ptr<T>&& asset);

        T* Get(AssetId id);

        void Clear()
        {
            data_.clear();
            idToIdx_.clear();
        }

    private:
        std::vector<std::unique_ptr<T>> data_;
        std::unordered_map<AssetId, uint32_t> idToIdx_;
    };

    template<typename T>
    void AssetRegistry<T>::Register(const char* name, std::unique_ptr<T>&& asset)
    {
        const auto id = GetAssetID(name);
        auto it = idToIdx_.find(id);
        if (it == idToIdx_.end())
        {
            idToIdx_[id] = data_.size();
            data_.emplace_back(std::move(asset));
        }
        else
        {
            TMT_WARN << "AssetID " << id << " already exists. Existing asset has been overwritten by " << name;
            data_[it->second] = std::move(asset);
        }
    }

    template<typename T>
    T* AssetRegistry<T>::Get(AssetId id)
    {
        auto it = idToIdx_.find(id);
        if (it == idToIdx_.end())
            TMT_ERR << "Invalid asset ID: " << id;

        return data_[it->second].get();
    }
}

#endif //TOMATO_ASSETREGISTRY_H
