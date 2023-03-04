#include <Nucleus/Nucleus.h>
#include <any>

using namespace Nucleus;

class Object {

public:

    void constFunc() const {

        Console::log("constFunc\n");

    }

    void func() {

        Console::log("func\n");

    }

};

int main(int argc, const char** argv) {
    
    ExceptionHandler::run([&](){

        List<int> myList = { 100, 200, 5312, 141, 1510, 1010, 014112, 0x414, 5154 };
        
        Bench::run([&](){

            Console::log("{0,x}\n{b}\n{0}\n", myList);

        }, 10000);

        nthrow("Exception!");

    });

    const auto data = File::load(argv[0]);

    File::write(String(argv[0]) + ".copy", data);

    int a = 5;
    void* b = nullptr;

    Function<void()> f = [&]() -> void {

        Console::log("Lambda! {} {}\n", a, b);

        // Random stuff to edit the variables' states
        b = reinterpret_cast<void*>(Random::randomInteger<intptr_t>(0, 0xffffffff));
        a = Random::randomInteger<int>(0, 50);

    };

    for (size_t i = 0; i < 50; ++i) {
        f();
    }

    using Signature = decltype(&Collection<int>::addAll);
    const Function<void(Object::*)()> classFunc = &Object::constFunc;
    const Function<Signature> polymorphicFunc = &Collection<int>::addAll;
    const Function<void(Object*)> lambda = [](auto* obj) {

        Console::log("{}\n", obj);

    };

    Function<void(Object*)> const& constRef = lambda;

    Object object;
    MutableArray<int> arr;
    MutableArray objArr = { &object };

    auto x = StaticArray<int, 4>({ 5, 6, 7, 8 });
    polymorphicFunc(arr, x);
    classFunc(object);
    constRef(objArr[0]);

    Console::log("{}\n", arr);
    
    return 0;

}
