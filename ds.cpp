
#include <iostream>

#include <vector>

using namespace std;

template <typename T>
class stack {
public:
	void push(T element) {
		data.push_back(element);
	}
	T pop() {
		T ret = data.back();
		data.pop_back();
		return ret;
	}
	T top() const {
		return data.back();
	}
	bool empty() {
		return data.empty();
	}
	int size() {
		return data.size();
	}

private:
	vector<T> data;
};

template <typename T>
class queue {
public:
	void push(T element) {
		data.push_back(element);
	}
	T pop() {
		T ret = data.front();
		data.erase(data.begin());
		return ret;
	}
	T front() const {
		return data.front();
	}
	bool empty() {
		return data.empty();
	}
	int size() {
		return data.size();
	}

private:
	vector<T> data;
};

struct pos {
	pos(float _x, float _y) {
		x = _x;
		y = _y;
	}
	float x, y;
};

int main() {

	stack<int> s;
	queue<int> q;

	vector<int> v(10);

	for (int i = 0; i < 10; i++)
		v[i] = i;
	for (int i = 0; i < 10; i++)
		cout << v[i] << endl;

	for (int i = 0; i < 10; i++)
		s.push(i);
	for (int i = 0; i < 10; i++) {
		cout << s.top() << endl;
		s.pop();
	}

	for (int i = 0; i < 10; i++)
		q.push(i);
	for (int i = 0; i < 10; i++) {
		cout << q.front() << endl;
		q.pop();
	}	

	v.push_back(1);
	v.push_back(2);

	int* address_first = &v.front();
	int* address_second = &v.back();

	cout << "first: " << address_first << endl;
	cout << "second: " << address_second << endl;
	cout << "second - first: " << address_second - address_first << endl;

	/*
	for (int i = 0; i < 10; i++)
		v.push_back(pos(i, i*2));

	// 0 1 2 3 4 5 6 7 8 9
	
	v[3] = pos(0, 134);

	for (int i = 0; i < 10; i++) {
		cout << v[i].x << " " << v[i].y << endl;
	}
	*/

	system("pause");
}