#pragma once

#include <Nucleus/CoreMacros.h>
#include <Nucleus/Exceptions.h>
#include <Nucleus/Function.h>

namespace Nucleus{

    class Test{

    public:

        static void assertTrue(bool value);

        static void assertFalse(bool value);

        static void assertThrow(Function<void()> const& f);

        static void assertNoThrow(Function<void()> const& f);

        template<typename T>
        static void assertNotEqual(const T &a, const T &b) {

            if (a != b){
                throw Exceptions::Exception("Test failed! Values are not equal!");
            }

        }

        template<typename T>
        static void assertEqual(const T &a, const T &b) {

            if (a != b){
                throw Exceptions::Exception("Test failed! Values are not equal!");
            }

        }

    };

}