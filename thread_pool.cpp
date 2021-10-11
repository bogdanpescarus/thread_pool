#include "thread_pool.hpp"

ThreadPool::ThreadPool(int numThreads) {
    start(numThreads);
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::start(int numThreads) {
    for(int i = 0; i < numThreads; ++i) {
		mThreads.emplace_back([=] () ->  void {
			while(true) {
				Task _task;
				{
					/* this lock is not needed when executing the _task(), 
					   but only while manipulating the queue and also when working
					   with the condition variable.
					   So, when _task() should be executed outside of the scope of 
					   the unique lock. */
					std::unique_lock<std::mutex> ul(mMtx);

					mCv.wait(ul, [=] { return (mStop || !mTasks.empty()); });

					/* We need the second condition so that the we wait for all 
					   of the tasks to be finished (and removed from the queue) */
					if(mStop && mTasks.empty()) {
						break;
					}

					_task = mTasks.front();
					mTasks.pop();
				}
				_task();
			}
		});
	}
}

void ThreadPool::stop() noexcept {
	std::unique_lock<std::mutex> ul(mMtx);
	mStop = true;
	ul.unlock();

	mCv.notify_all();
	/* it needs auto& (or std::thread&) and not just auto (or std::thread), because
	   the copy constructor of thread class was deleted */
	for(std::thread& thread : mThreads) {
		thread.join();  
    }  
}

void ThreadPool::enqueue(Task _task) {
	std::unique_lock<std::mutex> ul(mMtx);
	mTasks.push(_task);
	ul.unlock();

	mCv.notify_one();
}
