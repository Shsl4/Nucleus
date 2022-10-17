#pragma once

#include "MutableArray.h"
#include "Shared.h"
#include <utility>

namespace TinyCore {

    template<typename T>
    class ManagedArray : public MutableArray<Shared<T>> {

    public:

        using Super = MutableArray<Shared<T>>;

        ManagedArray() = default;

        explicit ManagedArray(size_t cap) : Super(cap) {

        }

        ManagedArray& operator+=(T const& element) {
            return dynamic_cast<ManagedArray &>(this->append(Shared<T>::make(element)));
        }

        ManagedArray& operator+=(T&& element) noexcept {
            return dynamic_cast<ManagedArray &>(this->append(Shared<T>::make(std::move(element))));
        }

    };

}