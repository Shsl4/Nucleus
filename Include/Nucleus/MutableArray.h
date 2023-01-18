#pragma once

#include <Nucleus/Collection.h>

namespace Nucleus {

    template<class T>
    class MutableArray : public Collection<T>{

        using Super = Collection<T>;

    public:

        MutableArray() = default;

        explicit MutableArray(size_t size);

        MutableArray(MutableArray const& other);

        MutableArray(MutableArray&& other) noexcept;

        MutableArray(std::initializer_list<T> list);

        ~MutableArray() override = default;

        typename Super::Iterator begin() const override;

        typename Super::Iterator end() const override;

        NODISCARD size_t size() const override;

        NODISCARD size_t capacity() const override;

        T &get(size_t index) const override;

        auto add(const T &element) -> decltype(*this)& override;

        auto addAll(const Collection<T> &array) -> decltype(*this)& override;

        auto insert(const T &element, size_t index) -> decltype(*this)& override;

        auto insertAll(const Collection<T> &array, size_t index) -> decltype(*this)& override;

        bool removeAt(size_t index) override;

        bool removeAllOf(const T &element) override;

        bool contains(const T &element) const override;

        void clear() override;

        NODISCARD bool isEmpty() const override;

        MutableArray& operator=(MutableArray const& other);

        MutableArray& operator=(MutableArray&& other) noexcept;

    private:

        void extend(size_t size = 1);

        T* buffer = nullptr;
        size_t bufferSize = 0;
        size_t bufferCapacity = 0;

    };

}

#define MUTABLE_INLINE
#include <Nucleus/Inline/MutableArray.inl>
#undef MUTABLE_INLINE