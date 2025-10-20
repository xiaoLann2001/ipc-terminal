#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <algorithm>
#include "Any.h"   // 引入自实现的 Any

// ---------------------------
// 通用事件结构体
// ---------------------------
struct Event {
    std::string name;   // 事件名称
    Any data;           // 任意类型数据
    std::string origin; // 来源模块

    Event(const std::string& n = "", const Any& d = Any(), const std::string& o = "")
        : name(n), data(d), origin(o) {}
};

// ---------------------------
// 前向声明
// ---------------------------
class Publisher;
class Observer;

// ---------------------------
// 观察者接口
// ---------------------------
class Observer {
public:
    virtual ~Observer() {}

    virtual void subscribe(Publisher* pub);
    virtual void unsubscribe(Publisher* pub);

    virtual void update(const std::string& sender, Event& evt) = 0;
};

// ---------------------------
// 发布者类
// ---------------------------
class Publisher {
public:
    explicit Publisher(const std::string& name)
        : name_(name), blocked_(false) {}

    virtual ~Publisher() {}

    std::string name() const { return name_; }

    void attach(Observer* obs) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (obs && std::find(observers_.begin(), observers_.end(), obs) == observers_.end()) {
            observers_.push_back(obs);
        }
    }

    void detach(Observer* obs) {
        std::lock_guard<std::mutex> lock(mutex_);
        observers_.erase(std::remove(observers_.begin(), observers_.end(), obs), observers_.end());
    }

    void blockNotify(bool block) {
        std::lock_guard<std::mutex> lock(mutex_);
        blocked_ = block;
    }

    void notify(const std::string& eventName, const Any& data = Any()) {
        std::vector<Observer*> temp;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (blocked_) return;
            temp = observers_;
        }

        Event evt(eventName, data, name_);
        for (auto* obs : temp) {
            if (obs) obs->update(name_, evt);
        }
    }

private:
    std::string name_;
    std::vector<Observer*> observers_;
    std::mutex mutex_;
    bool blocked_;
};

// ---------------------------
// 订阅/取消订阅实现
// ---------------------------
inline void Observer::subscribe(Publisher* pub) {
    if (pub) pub->attach(this);
}

inline void Observer::unsubscribe(Publisher* pub) {
    if (pub) pub->detach(this);
}
