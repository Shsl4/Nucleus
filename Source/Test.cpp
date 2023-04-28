#include <Nucleus/Test.h>

namespace Nucleus {

    void Test::assertTrue(bool value) {

        if (!value){
            throw Exceptions::Exception("Test failed! Expected true but received false!");
        }

    }

    void Test::assertFalse(bool value) {

        if (!value){
            throw Exceptions::Exception("Test failed! Expected true but received false!");
        }

    }

    void Test::assertThrow(const Function<void()> &f) {

        try{
            f();
        }
        catch(std::exception const&){
            return;
        }

        throw Exceptions::Exception("Test failed! Function did not throw when it was supposed to!");

    }

    void Test::assertNoThrow(const Function<void()> &f) {

        try{
            f();
        }
        catch(std::exception const&){
            throw Exceptions::Exception("Test failed! Function threw when it was not supposed to!");
        }


    }

}
