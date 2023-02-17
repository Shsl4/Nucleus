#include <Nucleus/Nucleus.h>

using namespace Nucleus;

List<int> myList;

int main(int argc, const char** argv) {

    ExceptionHandler::run([](){

        myList += { 100, 200, 5312, 141, 1510, 1010, 014112, 0x414, 5154 };
        
        Bench::run([](){

            Console::log("{0,x}\n{0,b}\n{0}\n", myList);

        }, 100000);

        throw Exceptions::Exception("Exception!");

    });

    auto data = File::load(argv[0]);

    File::write(String(argv[0]) + ".copy", data);

    return 0;

}