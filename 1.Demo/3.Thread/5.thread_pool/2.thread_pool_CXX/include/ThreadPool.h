#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "SafeQueue.h"

// 知识点补充:
// &&: 这里说的&&不是"与"的意思,而是是C ++ 11标准中定义的新引用运算符。 int && a表示“ a”是r-value引用。 &&通常仅用于声明函数的参数。 而且它只需要一个r值表达式。
//     简而言之，r-value是没有内存地址的值。 例如。 数字6和字符“v”都是r-value。 int a，a是一个l-value，但是（a + 2）是一个r值。 引自:https://www.tutorialspoint.com/What-is-double-address-operator-and-and-in-Cplusplus
// ...: 多个参数

class ThreadPool {
	public:
		ThreadPool(const int n_threads)
			: m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false) {
			}
		
		ThreadPool(const ThreadPool &) = delete;
		ThreadPool(ThreadPool &&) = delete;
		
		ThreadPool & operator=(const ThreadPool &) = delete;
		ThreadPool & operator=(ThreadPool &&) = delete;
		
		// Inits thread pool
		void init() {
			for (int i = 0; i < m_threads.size(); ++i) {
				m_threads[i] = std::thread(ThreadWorker(this, i));
			}
		}
		
		// Waits until threads finish their current task and shutdowns the pool
		void shutdown() {
			m_shutdown = true;
			m_conditional_lock.notify_all();
			
			for (int i = 0; i < m_threads.size(); ++i) {
				if(m_threads[i].joinable()) {
					m_threads[i].join();
				}
			}
		}
		
		// Submit a function to be executed asynchronously by the pool
		template<typename F, typename...Args>
		auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
			// Create a function with bounded parameters ready to execute
			std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
			// Encapsulate it into a shared ptr in order to be able to copy construct / assign 
			auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
			
			// Wrap packaged task into void function
			std::function<void()> wrapper_func = [task_ptr]() {
			(*task_ptr)(); 
			};
			
			// Enqueue generic wrapper function
			m_queue.enqueue(wrapper_func);
			
			// Wake up one thread if its waiting
			m_conditional_lock.notify_one();
			
			// Return future from promise
			return task_ptr->get_future();
		}

	private:
		class ThreadWorker {
			public:
				ThreadWorker(ThreadPool * pool, const int id)
					: m_pool(pool), m_id(id) {
					}
				
				void operator()() {  // 可以实现把对象当函数使用,例如: ThreadWorker a;   a();
					std::function<void()> func;
					bool dequeued;
					while (!m_pool->m_shutdown) {
						{
							std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
							if (m_pool->m_queue.empty()) {
								m_pool->m_conditional_lock.wait(lock);
							}
							dequeued = m_pool->m_queue.dequeue(func);
						}
						if (dequeued) {
							func();
						}
					}
				}
			private:
				int m_id;
				ThreadPool * m_pool;
		};
		
		bool m_shutdown;
		SafeQueue<std::function<void()>> m_queue;
		std::vector<std::thread> m_threads;
		std::mutex m_conditional_mutex;
		std::condition_variable m_conditional_lock;

};
