//
// Created by Adam T. Cuellar on 8/26/22.
//

#ifndef VALUE_H
#define VALUE_H

#include <functional>
#include <set>
#include <vector>
#include <iostream>

template<typename T>
class Value {
    T value;
    std::string _op;
    std::function<void()> _backward;
    std::set<Value<T>> _prev;

public:
    T grad;

    Value(T value, std::set<Value<T>> children=std::set<Value<T>>(), const std::string& op="");

    explicit operator T() const {
        return value;
    };

    Value& operator=(T obj) {
        value = obj;
        return *this;
    }

    bool operator<(const Value<T> &other) const {
        return value < other.value;
    }

    bool operator<(T other) const {
        return value < other;
    }

    bool operator>(const Value<T> &other) const {
        return value > other.value;
    }

    bool operator>(T other) const {
        return value > other;
    }

    Value<T> operator+(Value<T> &obj) {
        Value<T> out = Value(value + obj.value, std::set<Value<T>>{*this, obj}, "+");
        auto backward = [&]() {
            grad += out.grad;
            obj.grad += out.grad;
        };
        out._backward = backward;
        return out;
    }

    Value<T> operator+(T obj) {
        Value<T> other = Value(obj);
        Value<T> out = Value<T>(value + other.value, std::set<Value<T>>{*this, other}, "+");
        auto backward = [&]() {
            grad += out.grad;
            obj.grad += out.grad;
        };
        out._backward = backward;
        return out;
    }

    Value<T> operator-(Value<T> &obj) {
        return this + (-obj);
    }

    Value<T> operator-(T obj) {
        return this + (-obj);
    }

    Value<T> operator*(Value<T> &obj) {
        Value<T> out = Value(value * obj.value, std::set<Value<T>>{*this, obj}, "*");
        auto backward = [&]() {
            grad += obj.value * out.grad;
            obj.grad += value * out.grad;
        };
        out._backward = backward;
        return out;
    }

    Value<T> operator*(T obj) {
        Value<T> other = Value(obj);
        Value<T> out  = Value(value * other.value, std::set<Value<T>>{*this, other}, "*");
        auto backward = [&]() {
            grad += obj.value * out.grad;
            obj.grad += value * out.grad;
        };
        out._backward = backward;
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Value<T>& obj) {
        os << obj.value;
        return os;
    }

    void backward() {
        std::vector<Value<T>> topo;
        std::set<Value<T>> visited;
        std::function<void(Value<T>&)> dfs = [&](Value<T>& v) {
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                for (auto prev : v._prev) {
                    dfs(prev);
                }
                topo.push_back(v);
            }
        };
        dfs(*this);

        grad = (T) 1;
        for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
            if(it->_backward) {
                it->_backward();
            }
        }
    }

};

template<typename T>
Value<T>::Value(T value, std::set<Value<T>> children, const std::string& op) {
    this->value = value;
    this->grad = 0;
    this->_prev = children;
    this->_op = op;
}

template<typename T, typename U>
Value<T> operator*(U obj, Value<T> &v) {
    return v * (T) obj;
}

#endif //VALUE_H
