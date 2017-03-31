
#include <iostream>
#include <thread>
#include <queue>
#include <string>
#include <mutex>

using namespace std;

queue<string> message_queue;
mutex mq_mutex;
condition_variable mq_condition;
bool logging = true;

void thread_log() {
	unique_lock<mutex> lock(mq_mutex);
	while (logging) {
		mq_condition.wait(lock);
		if (!message_queue.empty()) {
			//unique_lock<mutex> lock(mq_mutex);
			while (!message_queue.empty()) {
				string message = message_queue.front();
				message_queue.pop();
				cout << message << endl;
			}
		}
	}
}

void thread_one() {
	mq_mutex.lock();
	message_queue.push("starting thread one!");
	mq_mutex.unlock();
	mq_condition.notify_all();

	for (int i = 0; i < 1000000; i++) {
		mq_mutex.lock();
		message_queue.push("doing some work in thread one");
		message_queue.push("doing some work in thread one");
		message_queue.push("doing some work in thread one");
		message_queue.push("doing some work in thread one");
		mq_mutex.unlock();
		mq_condition.notify_all();
	}

	mq_mutex.lock();
	message_queue.push("ending thread one!");
	mq_mutex.unlock();
	mq_condition.notify_all();
}

void thread_two() {
	mq_mutex.lock();
	message_queue.push("starting thread two!");
	mq_mutex.unlock();
	mq_condition.notify_all();

	for (int i = 0; i < 1000000; i++) {
		mq_mutex.lock();
		message_queue.push("doing some work in thread two");
		message_queue.push("doing some work in thread two");
		message_queue.push("doing some work in thread two");
		message_queue.push("doing some work in thread two");
		mq_mutex.unlock();
		mq_condition.notify_all();
	}

	mq_mutex.lock();
	message_queue.push("ending thread two!");
	mq_mutex.unlock();
	mq_condition.notify_all();
}


int main() {

	thread log(thread_log);

	thread t1(thread_one);
	thread t2(thread_two);

	t1.join();
	t2.join();

	logging = false;
	log.join();

	system("pause");
	return 0;
}