#pragma once

#include <string>
#include <any>
#include <vector>
#include <mutex>
#include <algorithm>

// 通用事件结构体
template<typename Payload>
struct Event {
    std::string name;     // 事件名称
    Payload data;         // 数据
    std::string origin;   // 来源模块名

    Event(const std::string& n, const Payload& d, const std::string& o)
        : name(n), data(d), origin(o) {}
};

// 前向声明
template<typename Payload>
class Publisher;

// 观察者接口
template<typename Payload>
class Observer {
public:
    virtual ~Observer() = default;
    
    virtual void subscribe(Publisher<Payload>* pub) {
        if (pub) {
            pub->attach(this);
        }
    }

    virtual void unsubscribe(Publisher<Payload>* pub) {
        if (pub) {
            pub->detach(this);
        }
    }

    virtual void update(const std::string& sender, Event<Payload>& evt) = 0;
};

// 发布者类
template<typename Payload>
class Publisher {
public:
    explicit Publisher(const std::string& name)
        : name_(name) {}

    virtual ~Publisher() = default;

    std::string name() const { return name_; }

    void attach(Observer<Payload>* obs) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (obs && std::find(observers_.begin(), observers_.end(), obs) == observers_.end()) {
            observers_.push_back(obs);
        }
    }

    void detach(Observer<Payload>* obs) {
        std::lock_guard<std::mutex> lock(mutex_);
        observers_.erase(std::remove(observers_.begin(), observers_.end(), obs), observers_.end());
    }

    void blockNotify(bool block) {
        std::lock_guard<std::mutex> lock(mutex_);
        blocked_ = block;
    }

    void notify(const std::string& eventName, const Payload& data = Payload()) {
        std::vector<Observer<Payload>*> tempObservers;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (blocked_) return;
            tempObservers = observers_; // 拷贝一份，解锁后再通知
        }
        Event<Payload> evt(eventName, data, this->name());
        for (auto* obs : tempObservers) {
            if (obs) obs->update(this->name(), evt);
        }
    }

private:
    std::string name_;
    std::vector<Observer<Payload>*> observers_;
    std::mutex mutex_;
    bool blocked_ = false;
};
