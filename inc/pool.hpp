//
// Created by Tywael on 2024/11/20
//
// for comprehensive documentation, please refer to the README.md file

#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <utility>

template<typename TType>
class Pool {
    public:
        class Object {
            public:
                Object(TType* object, Pool *pool) : _object(object), _pool(pool) {};
                ~Object() {
                    if (_object != nullptr) 
                        _pool->release(_object);
                }
                Object(const Object&) = delete;
                Object& operator=(const Object&) = delete;
                Object(Object&& other) noexcept : _object(other._object), _pool(other._pool) {
                    other._object = nullptr;
                    other._pool = nullptr;
                }
                Object& operator=(Object&& other) noexcept {
                    if (this != &other) {
                        _object = other._object;
                        _pool = other._pool;
                        other._object = nullptr;
                        other._pool = nullptr;
                    }
                    return *this;
                }
                TType *operator->() {
                    return _object;
                }
            private:
                TType* _object;
                Pool* _pool;
        };
        Pool() = default;
        Pool(const std::size_t &size) {
            resize(size);
        }
        ~Pool() {
            for (char* alloc_ptr : _allocPtrs) {
                std::free(alloc_ptr);
            }
        }
        void resize(const std::size_t &size) {
            if (size <= _size) return;
            try {
                _pool.reserve(size);
            }  catch (const std::bad_alloc &e) {
                std::cerr << "Memory allocation failed: " << e.what() << std::endl;
                throw;
            }
            size_t diff = size - _size;
            char* alloc_ptr = static_cast<char*>(std::malloc(sizeof(TType) * diff));
            if (alloc_ptr == nullptr) {
                throw std::bad_alloc();
            }
            _allocPtrs.push_back(alloc_ptr);
            for (size_t i = 0; i < diff; i++) {
                _pool.push_back(reinterpret_cast<TType*>(alloc_ptr + i * sizeof(TType)));
            }
            _size = size;
        }

        template<typename... TArgs>
        Object acquire(TArgs&&... args) {
            TType *object = _pool.back();
            _pool.pop_back();
            new (object) TType(std::forward<TArgs>(args)...);
            return Object(object, this);
        }

    private:
        std::vector<TType *> _pool;
        std::vector<char *> _allocPtrs;
        size_t _size = 0;

        void release(TType *object) {
            object->~TType();
            _pool.push_back(object);
        }
};
