
#include <iostream>
#include <functional>
#include <cstdlib>
#include <ctime>

#define max(a, b) ((a) > (b) ? a : b)

using namespace std;


template<typename T> class BST;

template<typename T>
class node {
	node(T val = 0) {
		left = right = NULL;
		value = val;
	}
	~node() {}

	T value;
	node *left, *right;
	friend class BST<T>;
};

template<typename T>
class BST {
public:
	BST() {
		root = NULL;
	}
	~BST() {
		clear();
	}

	int height() {
		if (!root) return 0;
		return height_rec(root);
	}

	void insert(T value) {
		if (!root) {
			root = new node<T>(value);
		}
		else {
			node<T> *current = root, *parent = NULL;
			while (current) {
				parent = current;
				if (value < current->value) {
					current = current->left;
				}
				else {
					current = current->right;
				}
			}
			if (value < parent->value) {
				parent->left = new node<T>(value);
			}
			else {
				parent->right = new node<T>(value);
			}
		}
	}
	bool remove(T value) {
		node<T> *current = root, *parent = NULL;
		while (current && current->value != value) {
			parent = current;
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		if (!current) {
			return false;
		}
		while (true) {
			if (!current->left && !current->right) {
				if (parent->left == current) {
					parent->left = NULL;
				}
				else {
					parent->right = NULL;
				}
				delete current;
				return true;
			}
			else {
				node<T>* successor = current->right;
				if (!successor) {
					if (parent->left == current) {
						parent->left = current->left;
					}
					else {
						parent->right = current->left;
					}
					delete current;
					return true;
				}
				parent = current;
				while (successor->left) {
					parent = successor;
					successor = successor->left;
				}
				T temp = current->value;
				current->value = successor->value;
				successor->value = temp;
				current = successor;
			}
		}
	}
	bool test(T value) {
		node<T>* current = root;
		while (current && current->value != value) {
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return current != NULL;
	}

	void preorder(function<void(T)> operation) {
		preorder_rec(root, operation);
	}
	void postorder(function<void(T)> operation) {
		postorder_rec(root, operation);
	}
	void inorder(function<void(T)> operation) {
		inorder_rec(root, operation);
	}

	void clear() {
		if(root)
			clear_rec(root);
		root = NULL;
	}

private:
	void preorder_rec(node<T>* current, function<void(T)> operation) {
		operation(current->value);
		if(current->left) preorder_rec(current->left, operation);
		if(current->right) preorder_rec(current->right, operation);
	}
	void postorder_rec(node<T>* current, function<void(T)> operation) {
		if (current->left) postorder_rec(current->left, operation);
		if (current->right) postorder_rec(current->right, operation);
		operation(current->value);
	}
	void inorder_rec(node<T>* current, function<void(T)> operation) {
		if (current->left) inorder_rec(current->left, operation);
		operation(current->value);
		if (current->right) inorder_rec(current->right, operation);
	}
	void clear_rec(node<T>* current) {
		if (current->left) clear_rec(current->left);
		if (current->right) clear_rec(current->right);
		delete current;
	}

	int height_rec(node<T>* current) {
		if (!current) return 0;
		return max(height_rec(current->left), height_rec(current->right)) + 1;
	}

	node<T>* root;
};


int main() {

	srand(time(NULL));
	
	BST<int> bst;

	bst.insert(50);
	for (int i = 0; i < 100; i++) {
		bst.insert(rand() % 300);
	}

	bst.preorder([](int i) -> void { cout << i << " "; });
	cout << endl << endl;
	bst.postorder([](int i) -> void { cout << i << " "; });
	cout << endl << endl;
	bst.inorder([](int i) -> void { cout << i << " "; });
	cout << endl << endl;

	int height = bst.height();
	cout << "height: " << height << endl;

	bool test = bst.test(50);
	cout << "test: " << test << endl;

	bool success = bst.remove(50);
	cout << "remove: " << success << endl << endl;

	bst.inorder([](int i) -> void { cout << i << " "; });
	cout << endl << endl;

	bst.clear();

	system("pause");
	return 0;
}