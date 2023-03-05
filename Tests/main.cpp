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

class MyClass3 {

public:
    
    bool operator==(const MyClass3&) const {
        return true;
    }

    int field = 0;
    
};


class MyClass2 {

private:

    friend Serializer<MyClass2>;
    
    MutableArray<int> myInts = { 1, 2, 5, 6, 8 };
    MutableArray<MyClass3> objects = { {}, {}, {}, {} };
    
};

class MyClass {

private:

    friend Serializer<MyClass>;
    
    bool myBool = false;
    String myName = "Object";
    Float64 myValue = 4531.4578;
    MyClass2 obj = {};
    
};

template<>
class Serializer<MyClass> {

public:
    
    static void serialize(Json::Object& archive, MyClass const& object) {
        
        archive.add("myBool", object.myBool);
        archive.add("myName", object.myName);
        archive.add("myValue", object.myValue);
        archive.add("obj", object.obj);
        
    }
        
    static MyClass deserialize(Json::Object const& object) {

        MyClass obj;

        obj.myBool = object.getBool("myBool");
        obj.myName = object.getString("myName");
        obj.myValue = object.getFloat<Float64>("myValue");
        obj.obj = object.getObject<MyClass2>("obj");

        return obj;
        
    }
    
};

template<>
class Serializer<MyClass2> {

public:
    
    static void serialize(Json::Object& archive, MyClass2 const& object) {
        
        archive.add("myInts", object.myInts);
        archive.add("objects", object.objects);
        
    }

        
    static MyClass2 deserialize(Json::Object const& object) {
        
        MyClass2 obj;

        obj.myInts = object.getArray<int>("myInts");
        obj.objects = object.getArray<MyClass3>("objects");

        return obj;
        
    }
    
};

template<>
class Serializer<MyClass3> {

public:
    
    static void serialize(Json::Object& archive, MyClass3 const& object) {
        
        archive.add("field",  Random::randomInteger(0, 500));
        
    }

        
    static MyClass3 deserialize(Json::Object const& object) {
        
        MyClass3 obj;

        obj.field = object.getInteger<int>("field");

        return obj;
        
    }
    
};


int main(int argc, const char** argv) {
    
    MyClass cool;
    Json json1;
    auto obj = Json::Object("root");
    
    Serializer<MyClass>::serialize(obj, cool);

    json1.rootObject.add(obj);
    
    MyClass cl = Serializer<MyClass>::deserialize(obj);
    
    Json json2;
    auto obj2 = Json::Object("root");
    Serializer<MyClass>::serialize(obj2, cl);
    json2.rootObject.add(obj2);

    json2.write("object.json");
    
    system("start object.json");
    
    Json json = Json::parse("example_2.json");
    json.write("copy.json");
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
