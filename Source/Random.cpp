#include <Nucleus/Random.h>

namespace Nucleus {

    bool Random::randomBool() {
        return static_cast<bool>(generator() & 1);
    }

    std::minstd_rand Random::createGenerator() {

        std::minstd_rand gen;

        UInt32 seed = static_cast<UInt32>(std::chrono::system_clock::now().time_since_epoch().count());

        gen.seed(seed);

        return gen;

    }

}