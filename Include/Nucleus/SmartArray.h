#pragma once

#include <Nucleus/Collection.h>
#include <utility>

namespace Nucleus {

    template<class T>
    class SmartArray : public Collection<T*> {

        using Super = Collection<T*>;

    public:

        SmartArray();

        explicit SmartArray(size_t capacity);

        SmartArray(SmartArray const& other);

        SmartArray(SmartArray&& other) noexcept;

        ~SmartArray() override;

        SmartArray& operator=(SmartArray const& other);

        SmartArray& operator=(SmartArray&& other) noexcept;

        NODISCARD typename Super::Iterator begin() const override;

        NODISCARD typename Super::Iterator end() const override;

        NODISCARD size_t size() const override;

        NODISCARD size_t capacity() const override;

        NODISCARD T *&get(size_t index) const override;

        auto add(T *const &element) -> decltype(*this)& override;

        auto addAll(const Collection<T *> &array) -> decltype(*this) & override;

        auto insert(T *const &element, size_t index) -> decltype(*this)& override;

        auto insertAll(const Collection<T *> &array, size_t index) -> decltype(*this) & override;

        bool removeAt(size_t index) override;
        
        void clear() override;
    
    private:

        void extend(size_t size);

        void decref();

        size_t arrayCapacity = 0;
        size_t arraySize = 0;
        T** buffer = nullptr;
        RefCounter* counter = nullptr;

    };

}

#define SMARTARRAY_INLINE
#include <Nucleus/Inline/SmartArray.inl>
#undef SMARTARRAY_INLINE