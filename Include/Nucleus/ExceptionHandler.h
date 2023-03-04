#pragma once

#include <Nucleus/Function.h>
#include <Nucleus/Console.h>

namespace Nucleus{

    class ExceptionHandler {

    public:

        static void run(const Function<void()> f){

            try {
                f();
            }
            catch(std::exception const& e) {

                Console::error("Unhandled error: {}\n", e.what());
                
                if(const auto* ex = dynamic_cast<const Exceptions::Exception*>(&e)) {
                    Console::error("Printing Stack Trace:\n{}\n", ex->formattedTrace());
                }
                
            }

        }

    };

}