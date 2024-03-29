﻿#pragma once

#include <Nucleus/String.h>
#include <Nucleus/Map.h>

namespace Nucleus {

    class Storage {

    public:

        virtual ~Storage() = default;

        NODISCARD virtual Storage* duplicate() const = 0;

    };

    class Value : public Storage {

    public:

        template<class T>
        NODISCARD T get() const;

        template<class T>
        void store(T const& element);

        NODISCARD Value* duplicate() const override;

        template<typename Integral> requires (std::is_integral_v<Integral> && !std::is_same_v<bool, Integral>)
        NODISCARD Integral get() const;

        template<typename Fp> requires std::is_floating_point_v<Fp>
        NODISCARD Fp get() const;

        template<class T> requires std::is_same_v<T, String>
        NODISCARD String get() const {
			return value.substring(1, value.size() - 1);
		}

        template<class T> requires std::is_same_v<T, bool>
        NODISCARD bool get() const{

			try {
         		return value.toBool();
         	}
         	catch (std::exception const&) {
	            throw Exceptions::Exception("Deserialization error");
			}

		}

        NODISCARD FORCEINLINE String rawValue() const { return this->value; }

   private:

        String value;

    };

}

#define STORAGE_INLINE
#include <Nucleus/Inline/Storage.inl>
#undef STORAGE_INLINE
