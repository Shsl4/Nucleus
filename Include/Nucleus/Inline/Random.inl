#pragma once

#ifdef RANDOM_INLINE

namespace Nucleus {

    template<typename NumberType>
    NumberType Random::randomInteger(NumberType min, NumberType max) {
        static_assert(std::is_integral_v<NumberType>, "NumberType must be an integral number.");
        return generator() % (max - min + 1) + min;
    }

    template<typename NumberType>
    NumberType Random::randomFloat(NumberType min, NumberType max) {
        static_assert(std::is_floating_point_v<NumberType>, "NumberType must be a floating point number.");
        NumberType scale = generator() / (NumberType) std::minstd_rand::max();
        return min + scale * (max - min);
    }

}

#endif
