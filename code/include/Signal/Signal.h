// Signal.h
#pragma once

// #define CALL_DIRECTLY
#define EVENT_LOOP

#if defined(CALL_DIRECTLY) && defined(EVENT_LOOP)
    #error "Both CALL_DIRECTLY and EVENT_LOOP cannot be defined simultaneously."
#elif !defined(CALL_DIRECTLY) && !defined(EVENT_LOOP)
    #error "Either CALL_DIRECTLY or EVENT_LOOP must be defined."
#endif

#ifdef CALL_DIRECTLY

#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <memory>

template <typename... Args>
class Signal {
public:
    using Slot = std::function<void(Args...)>;

    // 连接成员函数
    template <typename T>
    void connect(T* object, void (T::*method)(Args...)) {
        auto slot = [object, method](Args... args) {
            (object->*method)(args...);  // 调用成员函数
        };
        std::lock_guard<std::mutex> lock(mutex_);
        slots.push_back(std::make_shared<Slot>(slot));  // 存储槽
    }

    // 连接成员函数
    template <typename T>
    void connect(T* object, void (T::*method)(const Args&...)) {
        auto slot = [object, method](Args... args) {
            (object->*method)(args...);  // 调用成员函数
        };
        std::lock_guard<std::mutex> lock(mutex_);
        slots.push_back(std::make_shared<Slot>(slot));  // 存储槽
    }

    // 连接普通函数
    void connect(Slot slot) {
        std::lock_guard<std::mutex> lock(mutex_);
        slots.push_back(std::make_shared<Slot>(std::move(slot)));
    }

    // 断开连接
    void disconnect(Slot slot) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = std::remove_if(slots.begin(), slots.end(),
                                 [&slot](const std::shared_ptr<Slot>& s) { return *s == slot; });
        slots.erase(it, slots.end());
    }

    // 发射信号
    void emit(Args... args) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& slot : slots) {
            (*slot)(args...);  // 调用每个槽函数
        }
    }

private:
    std::vector<std::shared_ptr<Slot>> slots;
    std::mutex mutex_;  // 用于线程安全
};

#elif defined(EVENT_LOOP)

#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <memory>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

template <typename... Args>
class Signal {
public:
    using Slot = std::function<void(Args...)>;

    // 构造函数，启动事件循环线程
    Signal() : running(true) {
        eventLoopThread = std::thread(&Signal::eventLoop, this);  // 启动事件循环线程
    }

    // 析构函数，停止事件循环线程
    ~Signal() {
        stop();
        if (eventLoopThread.joinable()) {
            eventLoopThread.join();  // 等待事件循环线程退出
        }
    }

    // 连接普通函数或成员函数
    template <typename T>
    void connect(T* object, void (T::*method)(Args...)) {
        auto slot = [object, method](Args... args) {
            (object->*method)(args...);
        };
        std::lock_guard<std::mutex> lock(mutex_);
        slots.push_back(std::make_shared<Slot>(slot));
    }

    void connect(Slot slot) {
        std::lock_guard<std::mutex> lock(mutex_);
        slots.push_back(std::make_shared<Slot>(std::move(slot)));
    }

    // 发射信号
    void emit(Args... args) {
        std::lock_guard<std::mutex> lock(mutex_);
        // 将事件添加到队列
        for (auto& slot : slots) {
            eventQueue.push([slot, args...]() {
                (*slot)(args...);  // 执行槽函数
            });
        }
        condVar.notify_one();  // 通知事件调度器
    }

private:
    // 事件调度器，处理队列中的事件
    void eventLoop() {
        while (running) {
            std::unique_lock<std::mutex> lock(mutex_);
            condVar.wait(lock, [this]() { return !eventQueue.empty() || !running; });

            while (!eventQueue.empty()) {
                auto event = eventQueue.front();
                eventQueue.pop();
                lock.unlock();
                event();  // 执行事件
                lock.lock();
            }
        }
    }

    // 停止事件循环
    void stop() {
        running = false;
        condVar.notify_one();  // 唤醒事件循环线程，退出
    }

private:
    std::vector<std::shared_ptr<Slot>> slots;  // 存储连接的槽
    std::queue<std::function<void()>> eventQueue;  // 事件队列
    std::mutex mutex_;  // 用于线程安全
    std::condition_variable condVar;  // 用于线程同步
    std::atomic<bool> running;  // 事件循环是否继续运行
    std::thread eventLoopThread;  // 事件循环线程
};

#endif
