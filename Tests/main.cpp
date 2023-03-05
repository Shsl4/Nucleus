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

namespace Nucleus {

    class MyClass3 {

    public:
    
        bool operator==(const MyClass3& other) const {
            return field == other.field;
        }

        int field = 0;
    
    };


    class MyClass2 {

    public:

        bool operator==(const MyClass2& other) const {

            return myInts == other.myInts
                && objects == other.objects;
            
        }
    
        MutableArray<int> myInts = { 1, 2, 5, 6, 8 };
        MutableArray<MyClass3> objects = { {}, {}, {}, {} };
    
    };

    class MyClass {

    public:

        bool operator==(const MyClass& other) const {

            return myBool == other.myBool
                && myName == other.myName
                && Math::deq(myValue, other.myValue)
                && obj == other.obj;
            
        }
    
        bool myBool = false;
        String myName = "Object";
        Float64 myValue = 4531.4578;
        MyClass2 obj = {};
    
    };

    template<>
    class Serializer<MyClass> {

    public:
    
        static void serialize(Container* container, MyClass const& object) {

            container->add("myBool", object.myBool);
            container->add("myName", object.myName);
            container->add("myValue", object.myValue);
            container->add("obj", object.obj);
        
        }
        
        static MyClass deserialize(Container* container) {

            MyClass cl;
            
            cl.obj = container->get<MyClass2>("obj");
            cl.myBool = container->get<bool>("myBool");
            cl.myName = container->get<String>("myName");
            cl.myValue = container->get<Float64>("myValue");
            
            return cl;
        
        }
    
    };

    template<>
    class Serializer<MyClass2> {

    public:
    
        static void serialize(Container* container, MyClass2 const& object) {
        
            container->add("myInts", object.myInts);
            container->add("objects", object.objects);
        
        }

        
        static MyClass2 deserialize(Container* container) {

            MyClass2 obj;

            obj.myInts = container->get<MutableArray<int>>("myInts");
            obj.objects = container->getArray<MyClass3>("objects");
            
            return obj;
        
        }
    
    };

    template<>
    class Serializer<MyClass3> {

    public:
    
        static void serialize(Container* archive, MyClass3 const& object) {
        
            archive->add("field", object.field);
        
        }
        
        static MyClass3 deserialize(Container* object) {

            MyClass3 cool;

            cool.field = object->get<int>("field");
            
            return cool;
        
        }
    
    };
    
}

int main(int argc, const char** argv) {

    Json json = Json::parse("example_1.json");
    
    json.write("copy.json");

    MyClass obj;
    
    Archive archive;

    Serializer<MyClass>::serialize(archive.rootContainer(), obj);
    
    auto obj2 = Serializer<MyClass>::deserialize(archive.rootContainer());

    archive.reset();

    Serializer<MyClass>::serialize(archive.rootContainer(), obj2);

    auto obj3 = Serializer<MyClass>::deserialize(archive.rootContainer());
    
    Json(archive).write("object.json");

    assert(obj2 == obj3);

    system("start object.json");
    system("start copy.json");
    
    return 0;
    
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
