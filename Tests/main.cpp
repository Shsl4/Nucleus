#include <Nucleus/Nucleus.h>

using namespace Nucleus;

class Base {

    REFL_BODY(Base)
    
public:
    
    NODISCARD virtual String name() const { return "Base"; }
    
    virtual ~Base() = default;
    
};

class Derived : public Base {

    REFL_BODY(Derived)

 public:

    NODISCARD String name() const override { return "Derived"; }

};

namespace Nucleus {
    
    class MyClass4 {

        REFL_BODY(MyClass4)

    public:
        bool operator==(const MyClass4& other) const {
            return name == other.name;
        }

        String name = "Data";
        
    };

    class MyClass3 {

        REFL_BODY(MyClass3)

    public:
    
        bool operator==(const MyClass3& other) const {
            return field == other.field;
        }

        int field = 0;
        MutableArray<String> strings = { "first", "second", "third" }; 
        MutableArray<MutableArray<MyClass4>> strings2 = { { {} }, { {} }, { {} } }; 
        MutableArray<Float64> data = { Random::randomFloat(0.0, 1000.0), Random::randomFloat(0.0, 1000.0), Random::randomFloat(0.0, 1000.0) }; 
    
    };


    class MyClass2 {

        REFL_BODY(MyClass2)

    public:

        bool operator==(const MyClass2& other) const {

            return myInts == other.myInts
                && objects == other.objects;
            
        }
    
        MutableArray<int> myInts = { 1, 2, 5, 6, 8 };
        MutableArray<MyClass3> objects = { {}, {}, {}, {} };
    
    };

    class MyClass {

        REFL_BODY(MyClass)

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
    class Serializer<MyClass2> {

    public:
    
        static void serialize(Container* container, MyClass2 const& object) {
        
            container->add("myInts", object.myInts);
            container->add("objects", object.objects);
        
        }

        
        static MyClass2 deserialize(Container* container) {

            MyClass2 obj;

            obj.myInts = container->get<MutableArray<int>>("myInts");
            obj.objects = container->get<MutableArray<MyClass3>>("objects");
            
            return obj;
        
        }
    
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
    class Serializer<MyClass3> {

    public:
    
        static void serialize(Container* archive, MyClass3 const& object) {
        
            archive->add("field", object.field);
            archive->add("strings", object.strings);
            archive->add("strings2", object.strings2);
            archive->add("data", object.data);
        
        }
        
        static MyClass3 deserialize(Container* object) {

            MyClass3 cool;

            cool.field = object->get<int>("field");
            cool.strings = object->get<MutableArray<String>>("strings");
            cool.strings2 = object->get<MutableArray<MutableArray<MyClass4>>>("strings2");
            cool.data = object->get<MutableArray<Float64>>("data");
            
            return cool;
        
        }
    
    };

    template<>
    class Serializer<MyClass4> {

    public:
    
        static void serialize(Container* archive, MyClass4 const& object) {
        
            archive->add("name", object.name);

        }
        
        static MyClass4 deserialize(Container* object) {

            MyClass4 cool;

            cool.name = object->get<String>("name");
           
            return cool;
        
        }
    
    };
    
}

void mutableArrayTests() {

    // TODO: write tests
    
}

void stringTests() {

    auto fmt = String::format("a: {.2} {x} {b} {}. Hello, {}! {}.", 1.0, 2, 3, 4, "世界", "\\end");
    auto s = String("I hate C++").replaceOccurrences("hate", "love");
    auto s2 = String("aaabbbccc").replaceOccurrences("a", "b").replaceOccurrences("b", "c");

    Test::assertEqual(String(), String());
    Test::assertEqual(String::format("{}, {}", "Hello", "world!"), String("Hello, world!"));
    Test::assertEqual(String("Hello, world!").removeOccurrences("llo"), String("He, world!"));
    Test::assertEqual(String("Hello, world!").removeAll('l'), String("Heo, word!"));
    Test::assertEqual(String::format("{}", nullptr), String("nullptr"));
    Test::assertEqual(String::format("{x}", 10), String("0xa"));
    Test::assertEqual(String::format("{b}", 10), String("1010"));
    Test::assertEqual(String::format("{.3}", 13.134167), String("13.134"));
    Test::assertEqual(String::format("{.5}", 9.9999941146), String("9.99999"));
    Test::assertEqual(fmt, String("a: 1.00 0x2 11 4. Hello, 世界! \\end."));
    Test::assertEqual(String("0").toBool(), false);
    Test::assertEqual(String("true").toBool(), true);
    Test::assertEqual(String("41.52").toFloat64(), 41.52);
    Test::assertEqual(String("41416251").toInteger(), i64(41416251));
    Test::assertEqual(String("Cool String").replaceOccurrences("o", "hello"), String("Chellohellol String"));
    Test::assertEqual(s, String("I love C++"));
    
    s.replaceOccurrences("love", "hate").replaceOccurrences("C++", "Fortran");
    Test::assertEqual(s, String("I hate Fortran"));
    Test::assertEqual(s2, String("ccccccccc"));
    Test::assertEqual(s2.replaceOccurrences("c", ""), String());

}

void rawTests(){

    Test::assertEqual(Type::of<Raw<int**************************************&&>>(), Type::of<int>());
    Test::assertEqual(Type::of<Raw<String*&&>>(), Type::of<String>());
    Test::assertEqual(Type::of<Raw<const int* const&>>(), Type::of<const int>());
    Test::assertEqual(Type::of<Raw<MutableArray<int>*&>>(), Type::of<MutableArray<int>>());

}

void jsonTests() {

    MyClass obj;

    Json json;

    Archive& archive = json.archive();

    Serializer<MyClass>::serialize(archive.root(), obj);

    MyClass obj2 = Serializer<MyClass>::deserialize(archive.root());

    Test::assertEqual(obj, obj2);

    json.archive().reset();

    Serializer<MyClass>::serialize(archive.root(), obj2);

    json.write("object.json");

    json.archive().reset();

    Json j = Json::parse("object.json");

    MyClass obj3 = Serializer<MyClass>::deserialize(j.archive().root());

    Test::assertEqual(obj2, obj3);

}

void reflectionTest() {

    Base* obj = static_cast<Base*>(Class::fromName("Derived").unmanaged());

    auto cl = Class::of(obj);

    Test::assertEqual(obj->name(), String("Derived"));

    Test::assertEqual(cl, Class::fromName("Derived"));

}

int main(int argc, const char** argv) {

    ExceptionHandler::run([] {

        mutableArrayTests();
        stringTests();
        rawTests();
        jsonTests();
        reflectionTest();

    });

    return 0;

}
