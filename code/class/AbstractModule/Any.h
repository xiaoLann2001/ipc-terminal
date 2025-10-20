#pragma once
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <stdexcept>

/**
 * @brief 通用类型容器：实现类似 std::any 的功能（C++11 可用）
 * 
 * Any 可以存放任意类型的数据，并在运行时取出。
 * 内部使用「类型擦除」技术，通过基类指针隐藏具体类型。
 */
class Any {
public:
    /// 默认构造函数，表示空对象
    Any() : ptr_(nullptr) {}

    /**
     * @brief 模板构造函数，接收任意类型的值
     * 
     * 这里使用模板，将传入的类型 T 封装到内部的 Holder<T> 对象中，
     * 并存储一个指向抽象基类 BaseHolder 的智能指针，实现类型擦除。
     */
    template<typename T>
    Any(const T& value)
        : ptr_(new Holder<T>(value)) {}

    /**
     * @brief 拷贝构造函数
     * 
     * 当我们复制一个 Any 对象时，不知道它内部存放的具体类型。
     * 因此通过虚函数 clone() 在运行时创建一个新的 Holder<T> 拷贝，
     * 实现“深拷贝”而非浅指针复制。
     */
    Any(const Any& other)
        : ptr_(other.ptr_ ? other.ptr_->clone() : nullptr) {}

    /**
     * @brief 拷贝赋值运算符
     * 
     * 先判断自赋值，再使用 unique_ptr::reset() 管理旧内存，
     * 调用 clone() 创建新的副本。
     */
    Any& operator=(const Any& other) {
        if (this != &other) {
            ptr_.reset(other.ptr_ ? other.ptr_->clone() : nullptr);
        }
        return *this;
    }

    /**
     * @brief 类型安全的强制类型转换
     * 
     * 如果内部类型与 T 不匹配，则抛出 std::bad_cast 异常。
     * 否则将基类指针向下转型为 Holder<T>*，并返回存储的值。
     */
    template<typename T>
    T& cast() {
        if (!ptr_ || ptr_->type() != typeid(T))
            throw std::bad_cast();
        return static_cast<Holder<T>*>(ptr_.get())->value_;
    }

    /// const 版本的类型转换
    template<typename T>
    const T& cast() const {
        if (!ptr_ || ptr_->type() != typeid(T))
            throw std::bad_cast();
        return static_cast<Holder<T>*>(ptr_.get())->value_;
    }

    /// 判断是否存放了值（是否为空）
    bool has_value() const { return ptr_ != nullptr; }

    /// 返回存储值的类型信息（若为空返回 typeid(void)）
    std::type_index type() const { 
        return ptr_ ? ptr_->type() : typeid(void);
    }

private:
    /**
     * @brief 抽象基类（类型擦除的核心）
     * 
     * 所有实际类型的 Holder 都继承自它。
     * 通过虚函数提供统一接口：clone() 与 type()。
     */
    struct BaseHolder {
        virtual ~BaseHolder() {}
        virtual BaseHolder* clone() const = 0;
        virtual std::type_index type() const = 0;
    };

    /**
     * @brief 模板派生类，用于保存具体类型的值
     * 
     * 每个不同的 T 类型对应一个 Holder<T>。
     * - 保存实际的值 value_
     * - 实现 clone()（深拷贝自己）
     * - 返回自身类型信息 typeid(T)
     */
    template<typename T>
    struct Holder : BaseHolder {
        Holder(const T& v) : value_(v) {}
        BaseHolder* clone() const override { return new Holder(value_); }
        std::type_index type() const override { return typeid(T); }
        T value_; ///< 实际存储的数据
    };

    /**
     * @brief 指向多态基类的智能指针
     * 
     * 这是类型擦除的关键。
     * 通过 unique_ptr<BaseHolder>，我们不关心内部存储的是 Holder<int>、
     * Holder<std::string> 还是其他类型，只要能通过虚函数调用统一接口即可。
     */
    std::unique_ptr<BaseHolder> ptr_;
};
