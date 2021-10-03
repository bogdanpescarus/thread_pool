#include <iostream>
#include <thread>
#include <functional>
#include <vector>

#pragma once

#define max_threads 8

class ThreadPool {
	private:
		std::queue<std::function<void()> mTasks;
		std::vector<std::thread> mThreads;
		
	public:
		ThreadPool();

};
