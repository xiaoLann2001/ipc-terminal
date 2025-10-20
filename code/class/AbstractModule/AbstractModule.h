#pragma once
#include "AbstractObserverMode.h"

class AbstractModule : public Publisher, public Observer {
public:
    explicit AbstractModule(const std::string& name = "DefaultModule")
        : Publisher(name) {}

    virtual ~AbstractModule() = default;

    // 子类可重写处理事件
    virtual void update(const std::string& sender, Event& evt) override {}

    // 模块生命周期接口
    virtual bool init() = 0;
    virtual void deinit() = 0;
};
