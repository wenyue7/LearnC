/*************************************************************************
    > File Name: 2.1.thread_mutex_CXX.cpp
    > Author: LiHongjin
    > Mail: 872648180@qq.com
    > Created Time: 2024年03月12日 星期二 22时28分07秒
 ************************************************************************/

/*
 * references:
 * https://blog.csdn.net/u011726005/article/details/78253850
 * http://www.cnblogs.com/haippy/p/3284540.html
 *
 * 注意：
 * 1. 使用g++编译的时候需要加上 -lpthread 参数
 * 2. 该文件里写的互斥锁是C++库里使用的互斥锁，而不是linux下的互斥锁，
 *    linux中的互斥锁使用 pthread_mutex_t 来定义
 */

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <thread>
#include <cstdarg>

namespace thread_lock {

std::mutex g_io_mutex;

/* ------------------------------------------------------------------------ */

/* 1.使用std::mutex互斥锁来解决线程安全问题。 */

static void Func1() {
    g_io_mutex.lock();

    std::cout << "Enter thread: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Leave thread: " << std::this_thread::get_id() << std::endl;

    g_io_mutex.unlock();
}

static void Test1() {
    std::thread threads[10];
    for (std::thread& thread : threads) {
        thread = std::thread(Func1);
    }

    for (std::thread& thread : threads) {
        thread.join();
    }
}

/* ------------------------------------------------------------------------ */

template<typename T>
class Container {
public:
    void Add(const T& element) {
        mutex_.lock();
        elements_.push_back(element);
        mutex_.unlock();
    }

    /*
     * 2.同一个线程多次去获取已经被该线程获取的mutex锁，如果使用std::mutex就会导致程序进入死锁。
     * (A)解决办法可以使用std::recursive_mutex递归互斥量。
     * std::recursive_mutex 与 std::mutex 一样，也是一种可以被上锁的对象，
     * 但是和 std::mutex 不同的是，std::recursive_mutex 允许同一个线程
     * 对用一个互斥量多次上锁（即递归上锁），来获得对互斥量对象的多层所有权，
     * std::recursive_mutex 释放互斥量时需要调用与该锁层次深度相同次数
     * 的 unlock()，可理解为 lock() 次数和 unlock() 次数相同。而std::mutex
     * 效率更高。
     *
     */

    void AddForTestRecursive(const T& element) {
        mutex_.lock();
        Add(element);
        mutex_.unlock();
    }

    void Dump() const {
        mutex_.lock();
        for (const T& element : elements_) {
            std::cout << element << std::endl;
        }
        mutex_.unlock();
    }

private:
    std::list<T> elements_;
    /* 导致死锁。*/
    // std::mutex mutex_;
    /* 编译错误，因为Dump是const的，而函数中调用的mutex_的lock和unlock是非const的。*/
    // std::recursive_mutex mutex_;

    /*
     * (B)mutable是为了突破const的限制而设置的。被mutable修饰的变量，
     * 将永远处于可变的状态，即使在一个const函数中，甚至结构体变量
     * 或者类对象为const，其mutable成员也可以被修改。
     */
    mutable std::recursive_mutex mutex_;
};

static void Func2(Container<int>& container, int start, int end) {
    for (int i = start; i <= end; ++i) {
        container.AddForTestRecursive(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

static void Test2() {
    Container<int> container;

    std::thread thread1(Func2, std::ref(container), 1, 10);
    std::thread thread2(Func2, std::ref(container), 100, 110);

    thread1.join();
    thread2.join();

    container.Dump();
}

/* ------------------------------------------------------------------------ */

/*
 * 3.区域锁
 * 区域锁lock_guard使用起来比较简单，除了构造函数外没有其他member function，在整个区域都有效。
 * 区域锁unique_guard除了lock_guard的功能外，提供了更多的member_function，相对来说更灵活一些。
 */

static void Func3() {
    std::unique_lock<std::mutex> lock(g_io_mutex);
    std::cout << "Enter thread: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Leave thread: " << std::this_thread::get_id() << std::endl;
}

static void Test3() {
    std::thread threads[10];
    for (std::thread& thread : threads) {
        thread = std::thread(Func3);
    }

    for (std::thread& thread : threads) {
        thread.join();
    }
}

}  // namespace thread_lock

/* ------------------------------------------------------------------------ */

int main() {
    thread_lock::Test1();
    thread_lock::Test2();
    thread_lock::Test3();

    return 0;
}

