#pragma once

#include <Nucleus/CoreMacros.h>
#include <cmath>
#include <cfloat>

namespace Nucleus {

    class Math {

    public:

        template<typename NumberType>
        static constexpr NumberType pi = static_cast<NumberType>(3.14159265359);

        template<typename NumberType>
        FORCEINLINE static NumberType clamp(NumberType a, NumberType min, NumberType max) {
            return a > max ? max : (a < min ? min : a);
        }

        template<typename NumberType>
        FORCEINLINE static NumberType clampMin(NumberType a, NumberType min) {
            return a < min ? min : a;
        }

        template<typename NumberType>
        FORCEINLINE static NumberType clampMax(NumberType a, NumberType max) {
            return a > max ? max : a;
        }

        template<typename NumberType>
        FORCEINLINE static NumberType clampAround(NumberType a, NumberType min, NumberType max) {
            return a < min ? max - std::fabs(fmod(a - min, max)) : (a > max ? min + std::fabs(fmod(a, max)) : a);
        }

        template<typename NumberType>
        FORCEINLINE static NumberType max(NumberType a, NumberType b) {
            return a > b ? a : b;
        }

        template<typename NumberType>
        FORCEINLINE static NumberType min(NumberType a, NumberType b) {
            return a < b ? a : b;
        }

        template<typename NumberType>
        FORCEINLINE static NumberType max(NumberType a, NumberType b, NumberType c) {
            return a > b ? ((a > c) ? a : c) : ((b > c) ? b : c);
        }

        template<typename NumberType>
        FORCEINLINE static NumberType min(NumberType a, NumberType b, NumberType c) {
            return a < b ? ((a < c) ? a : c) : ((b < c) ? b : c);
        }

        FORCEINLINE static float cot(const float x) {
            return std::cos(x) / std::sin(x);
        }

        FORCEINLINE static float fastInvSqrt(float number) {

            const float x2 = number * 0.5f;
            constexpr float threehalfs = 1.5f;

            union {

                float f;
                uint32_t i;

            } conv = {number};

            conv.i = 0x5f3759df - (conv.i >> 1);
            conv.f *= threehalfs - (x2 * conv.f * conv.f);
            return conv.f;

        }

        template<typename NumberType>
        FORCEINLINE static NumberType toRadians(NumberType degrees) {
            return degrees * static_cast<NumberType>(0.01745329252f);
        }

        template<typename NumberType>
        FORCEINLINE static NumberType toDegrees(NumberType radians) {
            return radians * 180.0f / pi<NumberType>;
        }

        template<typename NumberType>
        FORCEINLINE static NumberType lerp(NumberType const &from, NumberType const &to, Float32 t) {
            return static_cast<NumberType>(from + (to - from) * t);
        }

        template<typename NumberType>
        FORCEINLINE static NumberType lerp(NumberType const &from, NumberType const &to, Float64 t) {
            return static_cast<NumberType>(from + (to - from) * t);
        }

        template<typename NumberType>
        FORCEINLINE static NumberType flip(NumberType const &v) {
            return static_cast<NumberType>(1) - v;
        }

        template<typename NumberType>
        FORCEINLINE static NumberType
        easeIn(NumberType const &from, NumberType const &to, float t, float easePower = 2.0f) {
            return lerp<NumberType>(from, to, powf(t, easePower));
        }

        template<typename NumberType>
        FORCEINLINE static NumberType
        easeOut(NumberType const &from, NumberType const &to, float t, float easePower = 2.0f) {
            return lerp<NumberType>(from, to, flip(powf(flip(t), easePower)));
        }

        template<typename NumberType>
        FORCEINLINE static NumberType
        easeInOut(NumberType const &from, NumberType const &to, float t, float easePower = 2.0f) {
            return lerp<NumberType>(from, to, lerp<NumberType>(powf(t, easePower), flip(powf(flip(t), easePower)), t));
        }

        FORCEINLINE static bool feq(const float a, const float b, const float epsilon = FLT_EPSILON) {
            return std::fabs(a - b) <= (std::fabs(a) > std::fabs(b) ? std::fabs(b) : std::fabs(a)) * epsilon;
        }

        FORCEINLINE static bool fneq(const float a, const float b, const float epsilon = FLT_EPSILON) {
            return !feq(a, b, epsilon);
        }

        FORCEINLINE static bool deq(const double a, const double b, const double epsilon = DBL_EPSILON) {
            return std::abs(a - b) <= (std::abs(a) > std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon;
        }

        FORCEINLINE static bool dneq(const double a, const double b, const double epsilon = DBL_EPSILON) {
            return !deq(a, b, epsilon);
        }

    };

}