
#include <unordered_map>
#include <list>
#include <iterator>

namespace q2_cache {

template <typename Value, typename Key = size_t> // = size_t unnecessary
struct bucket {
    using ListIt = typename std::list<std::pair<Key, Value>>::iterator;

    std::list<std::pair<Key, Value>> queue_;
    std::unordered_map<Key, ListIt> data_;
    size_t size_;
    
    //bucket(size_t size) : size_(size) {}
    bool full() const { return (queue_.size() == size_); }
};

template <typename Value, typename Key = size_t>
class Q2Cache { 
    using ListIt = typename std::list<std::pair<Key, Value>>::iterator;
    //using HashIt = typename std::unordered_map<Key, Value>::iterator; //?

    bucket<Value, Key> in;
    bucket<Value, Key> out;
    bucket<Value, Key> main;
public:

    Q2Cache& operator= (const Q2Cache& cache_tmp) = delete;
    Q2Cache(const Q2Cache&) = delete;

    Q2Cache(size_t size) { // Pareto rule
        if (size < 3)
            printf("Cache is too small!\n");

        //ternar operator
        main.size_ = size / 5 + 1;
        in.size_ = main.size_;
        out.size_ = size - main.size_ - in.size_;
        printf("In constructor: size - %d, main %d, in %d, out %d\n", size, main.size_, in.size_, out.size_);
    }

    template<typename slow_func>
    bool lookup_update(Key key, slow_func slow_get_page) {
        auto obj_place = main.data_.find(key); // iterator on hash table!!
        printf("main %d, in %d, out %d\n", main.size_, in.size_, out.size_);
        /*printf("[");
        std::cout << std::boolalpha << (typeid(obj_place) == typeid(main.data_.find(key)));
        printf("]\n");
        printf("hash_table: [%s]\n", typeid(obj_place).name());
        printf("list_end:   [%s]\n\n", typeid(main.data_.end()).name());

        printf("List Iter in HT: [%s]\n", typeid(in.data_[key]).name());
        printf("List Iter:       [%s]\n", typeid(in.queue_.begin()).name());

        printf("\nWe push - [%s]\nkey     - [%s]\n", typeid(std::make_pair(key, slow_get_page(key))).name(), typeid(in.queue_.front()).name());*/
        dump();
//So more copypast

        if (obj_place == main.data_.end()) {
            obj_place = in.data_.find(key);
            if (obj_place == in.data_.end()) {
                obj_place = out.data_.find(key);
                if (obj_place == out.data_.end()) {
                    if (in.full()) {  
                        if (out.full()) { // Is it good?
                            out.data_.erase(out.queue_.back().first);
                            out.queue_.pop_back();
                        }
                        in.data_.erase(in.queue_.back().first);
                        printf("Before out splice\n");

                        out.queue_.splice(out.queue_.begin(), in.queue_, std::prev(in.queue_.end())); // ?
                        out.data_[key] = out.queue_.begin();
                    }

                    in.queue_.push_front(std::make_pair(key, slow_get_page(key)));
                    in.data_[key] = in.queue_.begin();

                    return false;
                } 
                else {// If we find in out                  
                    if (main.full()) {
                        main.data_.erase(main.queue_.back().first);
                        main.queue_.pop_back();
                    }
                    printf("Before we increase main\n");
                    out.data_.erase(key); // ??
                    printf("A ");
                    printf("[%s] ", typeid(out.data_.find(key)->second).name());
                    if (out.data_.find(key) == out.data_.end()) auto b = out.data_.find(key)->second; //It mustn't work!!


                    printf("\nCCC");

                    if (out.data_.find(key)->second != out.queue_.end()) {
                        printf("They don't equal\n");
                        main.queue_.splice(main.queue_.begin(), out.queue_, out.data_.find(key)->second);//obj_place->second);  //?

                    }
                    printf("B");
                    main.data_[key] = main.queue_.begin();
                    printf("After we increase main\n");

                    return true;
                }
            }

            return true;
        }

        return true;
    }

    void dump() { //Copypast
        printf("main[%s]: ", typeid(main.queue_.begin()->second).name());
        for (ListIt i = main.queue_.begin(); i != main.queue_.end(); i++) {
            std::cout << "[" << i->second << "] -> ";
        }
        printf("\n");

        printf("in[%s]: ", typeid(in.queue_.begin()->second).name());
        for (ListIt i = in.queue_.begin(); i != in.queue_.end(); i++) {
            std::cout << "[" << i->second << "] -> ";
        }
        printf("\n");

        printf("out[%s]: ", typeid(out.queue_.begin()->second).name());
        for (ListIt i = out.queue_.begin(); i != out.queue_.end(); i++) {
            std::cout << "[" << i->second << "] -> ";
        }
        printf("\n\n");
    }
};

}