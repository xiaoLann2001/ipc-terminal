#pragma once

#include "AbstractObserverMode.h"

// 抽象模块类：既是发布者也是观察者
template<typename Payload>
class AbstractModule : public Publisher<Payload>, public Observer<Payload> {
public:
    explicit AbstractModule()
        : Publisher<Payload>("DefaultModule") {}

    explicit AbstractModule(const std::string& name)
        : Publisher<Payload>(name) {}

    virtual ~AbstractModule() = default;

    // 观察者接口实现：默认行为是忽略，可在子类重写
    virtual void update(const std::string& sender, Event<Payload>& evt) override {
        // 默认不做处理，可由子类实现具体逻辑
    }

    // 模块初始化接口（纯虚，必须实现）
    virtual bool init() = 0;

    // 模块释放接口（纯虚，必须实现）
    virtual void deinit() = 0;
};