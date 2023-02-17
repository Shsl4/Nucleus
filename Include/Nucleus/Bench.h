#pragma once

#include <Nucleus/Function.h>
#include <Nucleus/Console.h>

 namespace Nucleus {

     class Bench {

     public:

         static void run(Function<void()> f, size_t runCount = 100){

             using namespace std::chrono;

             auto begin = high_resolution_clock::now();

             for(size_t i = 0; i < runCount; ++i){ f(); }

             auto end = high_resolution_clock::now();
             auto duration = duration_cast<microseconds>(end - begin);
             auto value = static_cast<Float32>(duration.count()) / static_cast<Float32>(runCount);

             Console::log("Bench result: Average run time: {}µs / iteration\n", value);

         }

     };

}