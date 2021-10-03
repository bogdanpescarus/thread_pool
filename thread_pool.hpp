#include <iostream>
#include <thread>
#include <functional>
#include <vector>

#define max_threads 8

class ThreadPool {
	private:
		std::queue<std::function<void()> mTasks;
		std::vector<std::thread> mThreads;
		
	public:
		ThreadPool();

};
