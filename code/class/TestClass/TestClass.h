#pragma once

#include <iostream>
#include <string>
#include "AbstractModule.h"

// 测试类：继承自 AbstractModule
class TestClass : public AbstractModule<std::string> {
public:
    explicit TestClass(const std::string& name)
        : AbstractModule(name) {}

    // 实现模块初始化
    bool init() override {
        // 初始化逻辑
        notify("TestClass Initialized");
        return true;
    }

    // 实现模块释放
    void deinit() override {
        // 释放逻辑
        notify("TestClass Deinitialized");
    }

    // 实现观察者接口
    void update(const std::string& sender, Event<std::string>& evt) override {
        // 处理接收到的事件
        std::cout << this->name() << " - "
                  << "Received event from " << sender << ": "
                  << evt.name << " with data " << evt.data
                  << " (origin: " << evt.origin << ")" << std::endl;
        
        notify(this->name() + " Event Processed", evt.data);
    }
};