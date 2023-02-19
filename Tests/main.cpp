#include <Nucleus/Nucleus.h>

using namespace Nucleus;

List<int> MyList;

int main(int argc, const char** argv) {

    ExceptionHandler::run([](){

        MyList += { 100, 200, 5312, 141, 1510, 1010, 014112, 0x414, 5154 };
        
        Bench::run([](){

            Console::log("{0,x}\n{b}\n{0}\n", MyList);

        }, 10000);

        nthrow("Exception!");

    });

    const auto data = File::load(argv[0]);

    File::write(String(argv[0]) + ".copy", data);

    Console::flush();

    return 0;

}