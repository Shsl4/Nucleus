#pragma once

#ifdef RANDOM_INLINE

namespace Nucleus {

    bool Random::randomBool() {
        return static_cast<bool>(generator() & 1);
    }

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

    std::minstd_rand Random::createGenerator() {

        std::minstd_rand gen;

        UInt32 seed = static_cast<UInt32>(std::chrono::system_clock::now().time_since_epoch().count());

        gen.seed(seed);

        return gen;

    }

}

#endif
