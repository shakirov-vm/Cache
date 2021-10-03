
#pragma once

#include <iostream>

namespace q2_test {

bool intUnitTest(std::string input, std::string answer, int(*slow_get_page_int) (size_t));

struct page {
    int value_;
    size_t key_;
    size_t distance_;

    page(int value, size_t key, size_t distance) : value_(value), key_(key), distance_(distance) {} //?
};


class IdealCache { 
    using ListIt = std::list<page>::iterator;
    using HashIt = std::unordered_map<size_t, ListIt>::iterator;
    using VectIt = std::vector<int>::iterator;

    std::list<page> queue_;
    std::unordered_map<size_t, ListIt> data_;
    size_t size_;
    std::vector<int> potok_; // int
    size_t potok_pointer_;
    size_t potok_size_;

    IdealCache& operator= (const IdealCache& cache_tmp) = delete;
    IdealCache(const IdealCache&) = delete;

    bool full() const { return (queue_.size() == size_); }
    void dump();

public:

    IdealCache(size_t size, std::vector<int>& potok) : size_(size) {
        potok_ = potok;
        potok_size_ = potok_.size();
        potok_pointer_ = 0;
    }

    bool lookup_update(size_t key, int (*slow_get_page_int) (size_t));

    friend size_t ideal_hits(std::vector<int>& potok, int (*slow_get_page_int) (size_t), size_t capacity);
};

size_t ideal_hits(std::vector<int>& vector_potok, int (*slow_get_page_int) (size_t), size_t capacity);

}