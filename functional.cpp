
#include <cstdlib>
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std;

int global_thing = 0;

struct thing {
	thing() {
		this->x = 0;
		auto lambda = [this]() -> void {
			x = 5;
		};
		lambda();
	}
	int x;
};

int do_something_else(int& x) {
	x += 5;
	global_thing++;
	return rand();
}

int do_something(int value) {
	//cout << value << endl;
	return value - 1;
}

void do_something_and_print(int value, function<int(int)> something) {
	int new_value = something(value);
	cout << new_value << endl;
}

int do_something_and_return(int value, function<int(int)> something) {
	int new_value = something(value);
	return new_value - 2;
}

void do_lots_of_things(int value, function<int(int)> something, function<int(int, function<int(int)>)> things) {
	int new_value = things(value, something);
	cout << new_value << endl;
}

void do_recursion(int value) {
	if (value < 1) return;
	
	do_recursion(value - 1);
}

struct functor {
	functor(int i) {
		subtract_by = i;
	}
	int operator()(int value) {
		return value - subtract_by;
	}
	int subtract_by;
};

int add(int one, int two) {
	return one + two;
}


template<typename T>
vector<T> map(vector<T> values, function<T(T)> action) {
	vector<T> new_values;
	for (T value : values)
		new_values.push_back(action(value));
	return new_values;
}

int main() {

	int subtract_by = 5;

	vector<int> values_1 = { 1, 2, 3, 4, 5 };
	vector<int> values_2 = map<int>(values_1, [subtract_by](int i) -> int {return i - subtract_by; });
	for (int i : values_2)
		cout << i << endl;

	int x = 5;
	do_something_else(x);
	
	auto add_five = bind(add, 5, placeholders::_1);
	cout << add_five(3) << endl;
	cout << add(5, 3) << endl;
	
	functor minus_one(1);
	functor minus_three(3);
	cout << minus_one(5) << " " << minus_three(5) << endl;
	minus_one.subtract_by = -1;
	cout << minus_one(5) << " " << minus_three(5) << endl;

	function<int(int)> m3 = minus_three;
	function<int(int)> m5 = functor(5);
	cout << m3(5) << " " << m5(5) << endl;

	int (*pointer)(int);
	pointer = do_something;

	pointer(5);

	function<int(int)> f = do_something;
	f(5);
	
	do_something_and_print(5, f);

	function<int(int, function<int(int)>)> f2;
	f2 = do_something_and_return;
	do_lots_of_things(5, f, f2);

	int y = 5, z = 3;

	auto lambda = [&](int value) -> int {
		y++;
		return value - z;
	};

	int v = lambda(5);
	cout << v << " " << y << endl;

	function<int(int)> f3 = lambda;
	f3(5);
	
	system("pause");
	return 0;
}




