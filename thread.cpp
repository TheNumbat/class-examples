
#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <atomic>

using namespace std;

atomic<int> global_number_operations = 0;
mutex m;

void thread_func() {
	int number_operations = 0;
	for (int i = 0; i < 1000000; i++) {
		number_operations++;
		number_operations++;
		number_operations++;
		number_operations++;
	}
	global_number_operations += number_operations;
}

int main() {

	thread t1(thread_func);
	thread t2(thread_func);
	thread t3(thread_func);
	thread t4(thread_func);
	//thread t5(thread_func);
	//thread t6(thread_func);

	auto start_time = chrono::high_resolution_clock::now();

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	//t5.join();
	//t6.join();
	/*for (int i = 0; i < 4*4000000; i++)
		number_operations++;*/

	auto end_time = chrono::high_resolution_clock::now();

	cout << global_number_operations << endl;

	int ns = chrono::duration<int, nano>(end_time - start_time).count();

	cout << ns << endl;

	system("pause");
	return 0;
}