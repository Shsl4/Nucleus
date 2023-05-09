#pragma once

#include <Nucleus/Function.h>
#include <Nucleus/Console.h>

namespace Nucleus{

    class ExceptionHandler {

    public:

        static void run(Function<void()> const& f){

            try {
                f();
            }
            catch(Exceptions::Exception& e) {

                Console::error("Unhandled error: {}\n", e.what());
                Console::error("Printing Stack Trace:\n{}\n", e.formattedTrace());

            }

        }

    };

}