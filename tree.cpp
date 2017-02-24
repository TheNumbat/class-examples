
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>

using namespace std;

template<typename T> 
class heap {
public:
	heap();
	~heap();

	void insert(T value);
	T get_root();
	T remove_root();

	void clear();
	int size();

private:
	// ...
};


template<typename T> class tree;

template<typename T>
class node {
	node(node* p, T val = 0) {
		parent = p;
		value = val;
	}
	~node() {}

	T value;
	vector<node*> children;
	node* parent;
	friend class tree<T>;
};

template<typename T>
class tree {
public:
	tree() {
		cursor = root = NULL;
	}
	~tree() {
		clear(root);
		root = NULL;
	}

	void insert(T value) {
		if (cursor) {
			cursor->children.push_back(new node<T>(cursor, value));
		}
		else {
			root = cursor = new node<T>(NULL, value);
		}
	}
	T remove() {
		node<T>* parent = cursor->parent;
		if (parent) {
			for (node<T>* n : cursor->children) {
				n->parent = parent;
				parent->children.push_back(n);
			}
			parent->children.erase(find(parent->children.begin(), parent->children.end(), cursor));
			node<T>* temp = cursor;
			T tempval = temp->value;
			cursor = cursor->parent;
			delete temp;
			return tempval;
		}
		else {
			// you can't do this
			assert(false);
		}
	}
	T get() {
		if (cursor) return cursor->value;
		else return (T)0;
	}

	void go_up() {
		if (cursor && cursor->parent) cursor = cursor->parent;
	}
	void go_child(int index) {
		if (cursor && cursor->children.size() > index) cursor = cursor->children[index];
	}

	void preorder(function<void(T)> operation) {
		preorder_rec(root, operation);
	}
	void postorder(function<void(T)> operation) {
		postorder_rec(root, operation);
	}

private:
	void preorder_rec(node<T>* current, function<void(T)> operation) {
		operation(current->value);
		for (node<T>* n : current->children)
			preorder_rec(n, operation);
	}
	void postorder_rec(node<T>* current, function<void(T)> operation) {
		for (node<T>* n : current->children)
			postorder_rec(n, operation);
		operation(current->value);
	}

	void clear(node<T>* current) {
		for (node<T>* n : current->children) {
			clear(n);
		}
		delete current;
	}

	node<T>* cursor;
	node<T>* root;
};

void print_int(int i) {
	cout << i << " ";
}

int main() {

	tree<int> t;

	for (int i = 0; i < 10; i++) {
		t.insert(i);
		t.go_child(0);
	}

	t.postorder(print_int);
	cout << endl;

	for (int i = 0; i < 5; i++) {
		t.remove();
	}

	t.postorder(print_int);
	cout << endl;

	system("pause");
	return 0;
}