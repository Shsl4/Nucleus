#pragma once

#include <Nucleus/Function.h>
#include <Nucleus/Console.h>

namespace Nucleus{

    class ExceptionHandler {

    public:

        static void run(Function<void()> f){

            try {
                f();
            }
            catch(Exceptions::Exception const& e) {
                Console::error("Unhandled error: {}\nPrinting Stack Trace:\n{}\n", e.what(), e.formattedTrace());
            }

        }

    };

}