
#include <unordered_map>
#include <list>
#include <iterator>

namespace q2_cache {

    template <typename Value, typename Key = size_t> // = size_t unnecessary
    struct bucket {
        using ListIt = typename std::list<Value>::iterator;

        std::list<Key> queue_;
        std::unordered_map<Key, ListIt> data_;
        size_t size_;

        bool full() const { return (queue_.size() == size_); }
    };

    template <typename Value, typename Key = size_t>
    class Q2Cache {
        using ListIt = typename std::list<Value>::iterator;
        using HashIt = typename std::unordered_map<Key, Value>::iterator;

        bucket<Value, Key> in;
        bucket<Value, Key> out;
        bucket<Value, Key> main;
    public:

        Q2Cache& operator= (const Q2Cache& cache_tmp) = delete;
        Q2Cache(const Q2Cache&) = delete;

        Q2Cache(size_t size) { // Pareto rule
            if (size < 3)
               printf("Cache is too small!\n");
            main.size_ = size / 5 + 1;
            in.size_ = main.size_;
            out.size_ = size - main.size_ - in.size_;
        }

        template<typename slow_func>
        bool lookup_update(Key key, slow_func slow_get_page) {

            auto obj_place = main.data_.find(key); // iterator on hash table!!
            printf("[");
            std::cout << std::boolalpha << (typeid(obj_place) == typeid(main.data_.find(key)));
            printf("]\n");
            printf("hash_table: [%s]\n", typeid(obj_place).name());
            printf("list_end:   [%s]\n\n", typeid(main.data_.end()).name());

            printf("Key in HT: [%s]\n", typeid(in.data_[key]).name());
            printf("List Iter: [%s]\n", typeid(in.queue_.begin()).name());

            //Key eraised;
//So more copypast
            if (obj_place == main.data_.end()) {
                obj_place = in.data_.find(key);
                if (obj_place == in.data_.end()) {
                    obj_place = out.data_.find(key);
                    if (obj_place == out.data_.end()) {
                        if (in.full()) {
                            if (out.full()) { // Is it good?
                                //eraised = out.queue_.back();
                                out.data_.erase(out.queue_.back());
                                out.queue_.pop_back();
                            }
                            //eraised = in.queue_.back();
                            in.data_.erase(in.queue_.back());
 //                           out.queue_.splice(out.queue_.begin(), in, std::prev(in.queue_.end())); // ?
 //                           out.data_[key] = out.queue_.begin();
                        }
                        in.queue_.push_front(key);
 //                       in.data_[key] = in.queue_.begin();
                        return false;
                    } // If we find in out
                            
                    if (main.full()) {
                        //eraised = main.queue_.back();
                        main.data_.erase(main.queue_.back());
                        main.queue_.pop_back();
                    }

                    out.data_.erase(key); // ??
//                    main.queue_.splice(main.queue_.begin(), out, obj_place->second);
 //                   main.data_[key] = main.queue_.begin();
                    
                    //from out to main
                    //out.data_.erase(obj_place->second);
                    //splice()

                    return true;
                }

                return true;
            }

            return true;
        }
    };

}