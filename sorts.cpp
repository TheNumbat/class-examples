
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

void heapSort(vector<int>& data);
void insertionSort(vector<int>& data);
void radixSort(vector<int>& data);
vector<int> randoms();

int main() {

	srand(time(NULL));

	vector<int> data = randoms();
	for (int i : data) cout << i << ", ";
	cout << endl << endl;
	radixSort(data);
	for (int i : data) cout << i << ", ";
	cout << endl << endl;

	system("PAUSE");
}

vector<int> randoms() {
	vector<int> ret;
	for (int i = 0; i < 100; i++) {
		ret.push_back(rand() % 100000);
	}
	return ret;
}

void radixSort(vector<int>& data) {
	vector<vector<int>> buckets(10);

	for (int i = 0; i < 6; i++) {
		for (int num : data) {
			buckets[(num / (int)pow(10, i)) % 10].push_back(num);
		}

		data.clear();

		for (vector<int>& v : buckets) {
			data.insert(data.end(), v.begin(), v.end());
			v.clear();
		}
	}
}

void insertionSort(vector<int>& data) {
	for (int i = 1; i < data.size(); i++) {			// O(n)
		int j = i;										// O(1)
		while (j > 0) {									// O(n)
			if (data[j - 1] > data[j]) {					// O(1)
				int temp = data[j];							// O(1)
				data[j] = data[j - 1];						// O(1)
				data[j - 1] = temp;							// O(1)
				j -= 1;										// O(1)
			}
			else {
				j = 0;										// O(1)
			}
		}
	}
}

void heapSort(vector<int>& data) {
	for (int i = 0; i < data.size(); i++) {
		make_heap(data.begin() + i, data.end(), [](int a, int b) -> bool {return a > b; });
	}
}

