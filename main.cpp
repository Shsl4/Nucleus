#include "Allocator.h"
#include "StaticArray.h"
#include "ImmutableArray.h"

#include <iostream>

using namespace TinyCore;

#define randInt(min, max) ((rand() % ((max) - (min) + 1)) + (min))

void allocationTest1() {

	constexpr size_t allocSize = 10;
	int* array = Allocator<int>::allocate(allocSize);

	for(size_t i = 0; i < allocSize; ++i) {
		array[i] = randInt(0, 10);
	}

	for(size_t i = 0; i < allocSize; ++i) {
		std::cout << &array[i] << ": " << array[i] << std::endl;
	}

	std::cout << std::endl;

	constexpr size_t reallocSize = 5;

	Allocator<int>::reallocate(array, allocSize, reallocSize);

	for(size_t i = 0; i < reallocSize; ++i) {
		std::cout << &array[i] << ": " << array[i] << std::endl;
	}

	Allocator<int>::release(array);

}

class Object{

public:

	Object() : val(idx++), string("Object " + std::to_string(val)) {
	
	}

	Object(Object const& other){
		this->val = other.val;
		this->string = other.string;
	}

	void desc() const {
		std::cout << this << ": " << string << std::endl;
	}

private:

	int val = 0;
	std::string string;
	static inline int idx = 0;

};

void staticArrayTest() {

	auto arr = StaticArray<int, 5>();

	arr[4] = 3;

	for(auto& e : arr) {

		e = randInt(0, 50);
		std::cout << &e << ": " << e << std::endl;

	}

	StaticArray<int, 5> arr2 = arr;

	for(auto& e : arr2) {
		e = randInt(-5, 5);
		std::cout << &e << ": " << e << std::endl;
	}

} 

void immutableArrayTest() {

	ImmutableArray<Object> objects = ImmutableArray<Object>(50);

	for(auto const& e : objects){
		e.desc();
	}

	auto objects2 = std::move(objects);

	for(auto const& e : objects2){
		e.desc();
	}

}

int main(int argc, char** argv) {

	srand(time(nullptr));
	allocationTest1();
	staticArrayTest();
	immutableArrayTest();

	return 0;

}
