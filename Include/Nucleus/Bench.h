#pragma once

#include <Nucleus/Function.h>
#include <Nucleus/Console.h>

 namespace Nucleus {

    /*!
     * A simple utility class that allows function performance testing
     */
     class Bench {

     public:

         /*!
          * Runs a function multiple times and outputs its average execution time.
          * @param f The function to execute
          * @param runCount The number of times to execute
          */
         static void run(Function<void()> const& f, size_t runCount = 100){

             using namespace std::chrono;

             auto begin = high_resolution_clock::now();

             for(size_t i = 0; i < runCount; ++i){ f(); }

             auto end = high_resolution_clock::now();
             auto duration = duration_cast<microseconds>(end - begin);
             auto value = static_cast<Float64>(duration.count()) / static_cast<Float64>(runCount);

             Console::log("Bench result: Average run time: {}µs / iteration\n", value);

         }

     };

}