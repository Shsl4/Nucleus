#include <Nucleus/Nucleus.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <concepts>
#include "Nucleus/MutableArray.h"

using namespace Nucleus;

#define randInt(min, max) ((rand() % ((max) - (min) + 1)) + (min))

void allocationTest1() {

	constexpr size_t allocSize = 10;
	int* array = Allocator<int>::allocate(allocSize);

	for (size_t i = 0; i < allocSize; ++i) {
		array[i] = randInt(0, 10);
	}

	for (size_t i = 0; i < allocSize; ++i) {
		std::cout << &array[i] << ": " << array[i] << std::endl;
	}

	std::cout << std::endl;

	constexpr size_t reallocSize = 5;

	Allocator<int>::reallocate(array, allocSize, reallocSize);

	for (size_t i = 0; i < reallocSize; ++i) {
		std::cout << &array[i] << ": " << array[i] << std::endl;
	}

	Allocator<int>::release(array);

}

class Object {

public:

	Object() : val(idx++), string("Object " + std::to_string(val)) {

	}

	Object(Object&& other) {

		*this = std::move(other);

	}

	Object& operator=(Object&& other) {
		std::cout << "Move" << std::endl;
		this->val = other.val;
		this->string = other.string;
		other.val = 0;
		other.string = "";
		return *this;
	}

	Object& operator=(Object const& other) {
		std::cout << "Copy" << std::endl;
		this->val = other.val;
		this->string = other.string;
		return *this;
	}

	Object(Object const& other) {
		this->val = other.val;
		this->string = other.string;
	}

	void desc() const {
		std::cout << this << ": " << string << std::endl;
	}

	std::string toString() const {

		std::stringstream stream;
		stream << "{ " << idx << ", " << string << " }";
		return stream.str();

	}

private:

	int val = 0;
	std::string string;
	static int idx;

};

int Object::idx = 0;

void staticArrayTest() {

	auto arr = StaticArray<int, 5>();

	arr[4] = 3;

	for (auto& e : arr) {

		e = randInt(0, 50);
		std::cout << &e << ": " << e << std::endl;

	}

	StaticArray<int, 5> arr2 = arr;

	for (auto& e : arr2) {
		e = randInt(-5, 5);
		std::cout << &e << ": " << e << std::endl;
	}

}

void immutableArrayTest() {

	ImmutableArray<Object> objects = ImmutableArray<Object>(50);

	for (auto const& e : objects) {
		e.desc();
	}

	auto objects2 = std::move(objects);

	for (auto const& e : objects2) {
		e.desc();
	}

}

void mutableArrayTest() {

	MutableArray<int> ok(50);

	for (int i = 0; i < 50; ++i) {
		ok.add(randInt(0, 1));
	}

	for (auto const& e : ok) {
		std::cout << e << " ";
	}

	std::cout << std::endl;

	ok.insertAll(ok, 2);
    ok.remove(0);
    ok.remove(1);

	for (auto const& e : ok) {
		std::cout << e << " ";
	}

	std::cout << std::endl;

	std::cout << ok.size() << " " << ok.capacity() << std::endl;

}

void test() {

	StaticArray<int, 5> a{};
	a[4] = 2;

	ImmutableArray<int> b(5);

	MutableArray<int> c;

	c += 5;

	for (int i = 0; i < 500; ++i) {
		c.add(randInt(0, 30));
	}

	for (auto const& e : c) {
		std::cout << e << std::endl;
	}

	std::cout << "Array size: " << c.size() << std::endl;

}

void mapTest() {

	Map<String, int> map(10);

	map += { "Damien", 0 };
	map += { "Denis", 1 };
	map += { "Daniel", 2 };

	map["Damien"] = 5;

	for (auto& e : map) {

		std::cout << "{ " << e.getKey() << ", " << e.getValue() << " }" << std::endl;

	}

}

void splitTest() {

	const String r = " Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non purus elementum, scelerisque velit tristique, venenatis purus. Cras a bibendum purus. Suspendisse eget vestibulum neque. Sed pretium elit eu urna lacinia, vel elementum libero aliquet. Ut odio ipsum, bibendum ac purus quis, laoreet viverra arcu. Suspendisse non efficitur eros. Praesent convallis tincidunt lectus, eget cursus lacus feugiat at. Morbi odio velit, lacinia porttitor tempus in, semper quis turpis. Aenean vitae venenatis est. Proin in nulla tempor, ultricies odio rhoncus, auctor nibh. Nam cursus sapien id sollicitudin congue. Proin sit amet rhoncus massa. Proin efficitur, urna sed dignissim lacinia, urna velit tristique dolor, et scelerisque ligula magna et felis. Nunc dictum felis ex, id consectetur augue consectetur eget. Maecenas at tempor arcu. Nulla commodo, odio ac gravida varius, nisi nisl ullamcorper nisl, in feugiat nibh purus a sem. Curabitur ut condimentum ipsum. Pellentesque luctus metus id dolor mattis volutpat quis sit amet est. Ut at risus at enim porttitor mollis. Fusce nibh sem, pretium quis aliquet vel, imperdiet nec nunc. Nam tristique sollicitudin ligula. Nulla congue elit et velit vulputate, porta feugiat tellus luctus. In dignissim consequat feugiat. Praesent vehicula venenatis est ac aliquet. Sed pulvinar gravida pharetra. Fusce ut diam sed lacus mattis ullamcorper vitae a est. Cras pharetra felis at sapien hendrerit convallis. Nunc elit mauris, consectetur ac massa a, dapibus porta neque. Morbi non iaculis purus. Praesent at velit maximus, molestie ante vel, hendrerit purus. Nam dapibus magna nec nisl luctus faucibus. Donec mollis diam faucibus, finibus lorem facilisis, sagittis orci. Fusce vel feugiat urna, vel lacinia justo. Aenean at porttitor orci, ut facilisis enim. Nullam lacinia viverra lobortis. Fusce placerat, nunc viverra ultricies ultricies, libero lacus tincidunt felis, eu dignissim enim ipsum cursus magna. Maecenas ac nunc metus. Nam imperdiet enim enim. Praesent nec maximus dolor. Nullam non elementum felis, in sollicitudin risus. Duis id nibh id dolor lobortis rutrum at eget quam. Pellentesque sit amet finibus lectus. Sed eget ultrices tellus. Etiam id risus sem. Nunc non rutrum est, ac placerat enim. Suspendisse posuere risus felis, id auctor nibh interdum et. Integer suscipit venenatis auctor. Vivamus at leo sagittis, molestie elit in, egestas nibh. Etiam vitae consectetur dolor, elementum egestas nibh. Curabitur tempus tellus vel erat iaculis, eget pretium purus dignissim. Fusce ornare vel ex dignissim suscipit. Nullam a maximus mi. Aenean pellentesque mollis odio ac commodo. In ornare, metus vitae viverra consectetur, nisl nisl cursus felis, ut tempus lectus mi a ligula. Phasellus id mi id odio lobortis tristique. Sed ac vulputate nulla. In nisl mi, gravida vitae dapibus non, sagittis a odio. Maecenas a auctor neque, eu volutpat enim. Duis id mollis felis, eget posuere nibh. Sed efficitur velit sit amet nisi cursus, ac vestibulum sapien vehicula. Donec consequat ex et massa dapibus gravida. Nunc sit amet sagittis leo. Cras luctus elementum condimentum. Donec tincidunt eu elit in ullamcorper. In eu ante orci. Curabitur in diam sit amet massa ornare suscipit. Etiam id felis vel magna porttitor malesuada. Praesent vel condimentum ante. Donec magna enim, molestie nec luctus ac, pretium et arcu. Cras ut auctor justo. Quisque mollis, ipsum in ultrices sollicitudin, orci nunc cursus lectus, ac placerat nunc nibh nec ante. Duis efficitur urna eu auctor euismod. Praesent in tellus lacus. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Vivamus porttitor felis ac dui maximus efficitur. Aliquam ut ex ut ipsum commodo interdum. Vestibulum non ligula et orci pellentesque eleifend molestie sit amet lorem. Proin ac augue et orci dictum suscipit. Nam porttitor leo ex. Pellentesque est enim, finibus a lobortis ac, maximus et dolor. Morbi consectetur diam sit amet tristique mollis. Nam quis ultricies massa. Nulla eget nisi vitae orci posuere ornare. Cras gravida sem sodales fringilla fringilla. Sed tincidunt vel ligula at suscipit. Aenean eu urna sit amet elit scelerisque aliquet. In vestibulum condimentum venenatis. Aliquam bibendum ante ac quam bibendum tincidunt sed molestie neque. Proin facilisis rutrum mi nec luctus. Ut venenatis diam nunc, ut ultrices quam vulputate ac. Suspendisse consectetur aliquam lorem id vulputate. Quisque id ultrices lectus, in auctor quam. Mauris augue turpis, suscipit id porta ac, maximus et massa. Nulla pulvinar enim sit amet risus luctus, sed maximus nisi facilisis. Vivamus laoreet sapien dui, eu iaculis metus ultricies sed. Mauris tincidunt sollicitudin ultricies. Vivamus fermentum, ex et iaculis lacinia, arcu dui feugiat nunc, a fringilla quam leo ut mi. In hendrerit quam non nisl aliquam, a sollicitudin libero sodales. Suspendisse viverra, nulla nec imperdiet tempor, orci lorem ullamcorper nulla, vitae interdum enim risus et dui. Suspendisse ullamcorper magna et bibendum porta. Cras sit amet lorem eget lectus cursus luctus pretium non enim. Pellentesque varius risus et erat scelerisque cursus. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Vestibulum tincidunt lobortis velit nec scelerisque. Maecenas eget tortor vitae mauris fringilla lacinia at ut purus. Maecenas tempus condimentum erat, sed rhoncus quam malesuada vitae. Nullam semper massa aliquam dui interdum, eu volutpat orci suscipit. Sed id dolor quis augue pretium faucibus. Fusce efficitur pulvinar lorem vel dignissim. Mauris viverra enim vitae lorem semper interdum. Etiam tincidunt maximus leo sed posuere. Curabitur neque magna, consequat quis vulputate a, rutrum ut diam. Phasellus sollicitudin elementum purus in tempus. Integer malesuada arcu libero, eget congue ante vehicula eu. Nulla egestas consequat ligula, pulvinar rhoncus risus tempor sit amet. Aenean scelerisque sit amet turpis sed finibus. Cras accumsan dolor elit, at convallis leo ultricies at. Mauris ac hendrerit orci. Sed a posuere magna. Duis condimentum lectus id semper dictum. Quisque posuere non urna vitae euismod. Aliquam erat volutpat. Nullam libero sapien, lobortis eget nisl a, cursus suscipit tortor. Nunc pellentesque pretium dapibus. Cras eget sapien purus. Fusce ante orci, consequat eget diam non, scelerisque pretium elit. Nulla sit amet nibh eget magna tristique dapibus non eget tortor. Sed auctor feugiat viverra. Sed lobortis nisl rutrum erat hendrerit, non viverra felis feugiat. Nunc scelerisque quam id erat ultricies semper. Praesent sed nisl dapibus, dictum mi et, vestibulum orci. Aenean at quam id est ultrices. ";

	const auto o = r.split(L' ');

	for (auto const& w : o) {
		std::wcout << w << std::endl;
	}

}

class Parent {

public:

	Parent(String s) : val(std::move(s)) {

	}

	virtual ~Parent() = default;

	String getString() const { return val; }

	void setValue(String const& v) { this->val = v; }

private:

	String val;

};

class Child : public Parent {


public:

	Child(String s) : Parent(std::move(s)) {

	}

	void test() {
		std::cout << e << std::endl;
	}

private:

	String e = "Error";

};


class Shape
{
public:
    virtual ~Shape() {}
    NODISCARD virtual double get_area() const = 0;
};

class Circle : public Shape
{
public:
    Circle() : radius_(12.5) {}
    NODISCARD double get_area() const override { return 3.14 * radius_ * radius_; }

private:
    double radius_;

};

REFLECTABLE(Circle)
REFLECTABLE(String)
REFLECTABLE(Object)
TEMPLATE_REFLECTABLE(SmartArray, int)

int main(int argc, char** argv) {

	const auto ss = Shared<String>::make("Unique String!");
	const auto si = Unique<int>::make(9);

	Console::log("안녕하세요 {} 😊 {4} {5} {2} {} {6}\n", "世界", 48.0f, "World", String("Test"), ss, "Hello", si);

	bool bb = false;
	bool* bp = &bb;

	UInt64* llp = nullptr;

	Console::error("{} 😃 {} | 😨 {}\n", bp, bb, llp);

    SmartArray<int> arr;

    int* a = Allocator<int>::construct(5);
    int* b = Allocator<int>::construct(6);
    int* c = Allocator<int>::construct(7);
    int* d = Allocator<int>::construct(8);

    arr.add(a).add(b).add(c).add(d);

    arr.removeAt(0);

    Console::log("{}{}{}\n", Console::cyan, arr.contains(a), Console::reset);

    Any console = ReflectionFactory::createObject("ExplorerConsole");
    Any sh = ReflectionFactory::createObject("Circle");
    Any arr2 = ReflectionFactory::createObject("SmartArray<int>");
    Any str = ReflectionFactory::createObject("String");
    
    *str.get<String>() = "Hello";

    Console::log("{}\n", str.get<String>());
    
    auto& g = *arr2.get<SmartArray<int>>();

    g.add(new int(5)).add(new int(2));

    Console::log("{}\n", sh.get<Circle>()->get_area());
    Console::log("{} {}\n", g[0], g[1]);

    Function<int()> func = [](){ return 5; };
    ErasedFunction erased = func.erased();

    auto ch = Child("Hi");

    Function<void(Child::*)(void)> fun = &Child::test;
    fun.invokeWith(ch);

    Console::log("{2}\n", erased.invoke<int>(), 8, 9);

    MutableArray<int> myIntArray;

    auto& ww = myIntArray += { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto e = myIntArray -= { 6, 3 };

    myIntArray -= myIntArray;

    auto w = myIntArray + 6;

    w += { 1, 2, 3, 4, 5 };

    w.randomize();

    Console::log("{}\n", myIntArray);
    Console::log("{}\n", w);

	return 0;

}

