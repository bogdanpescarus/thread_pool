#include <iostream>
#include <thread>
#include <functional>
#include <condition_variable>
#include <vector>
#include <queue>

#define max_threads 8

class ThreadPool {
	private:
		using Task = std::function<void()>;
		std::vector<std::thread> mThreads;

		/* this is for the actual tasks that need to use 
		   the thread pool */
		std::queue<Task> mTasks;

		std::mutex mMtx;
		std::condition_variable mCv;
		bool mStop = false;
		
	public:
		

		ThreadPool(int);
		
		void start(int);
		void stop() noexcept;

		void enqueue(Task);

		~ThreadPool();
};
