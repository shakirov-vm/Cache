
#include <unordered_map>
#include <list>
#include <iterator>

#define DEBUG 0

namespace q2_cache {

    template <typename Value, typename Key = size_t> // = size_t unnecessary
    struct bucket {
        using ListIt = typename std::list<std::pair<Key, Value>>::iterator;
        using HashIt = typename std::unordered_map<Key, ListIt>::iterator;

        std::list<std::pair<Key, Value>> queue_;
        std::unordered_map<Key, ListIt> data_;
        size_t size_;

        bool full() const { return (queue_.size() == size_); }
        void dump() const {
            printf("[%s]: ", typeid(queue_.begin()->second).name());
            for (auto i = queue_.begin(); i != queue_.end(); i++) {
                std::cout << "[" << i->second << "] -> ";
            }
            printf("\n");
        }
        bool finded_in_HT(Key key) {
            HashIt key_HT_Iter = data_.find(key);
            if (key_HT_Iter != data_.end()) {

                if (key_HT_Iter->second != queue_.begin())
                    queue_.splice(queue_.begin(), queue_, key_HT_Iter->second);

                if (DEBUG) dump();
                return true;
            }
            return false;
        }
    };

    template <typename Value, typename Key = size_t>
    class Q2Cache {
        using ListIt = typename std::list<std::pair<Key, Value>>::iterator;
        using HashIt = typename std::unordered_map<Key, ListIt>::iterator;

        bucket<Value, Key> in;
        bucket<Value, Key> out;
        bucket<Value, Key> main;
    public:

        Q2Cache& operator= (const Q2Cache& cache_tmp) = delete;
        Q2Cache(const Q2Cache&) = delete;

        Q2Cache(size_t size) { // Pareto rule
            if (size < 3) {
                printf("Cache is too small! If size less than 3 - this will work uncorrect\n");
                exit(-1); // Is there a more correct way to exit the program (from constructor)?
            }

            main.size_ = size / 5 + 1;
            in.size_ = main.size_;
            out.size_ = size - main.size_ - in.size_;
        }

        template<typename slow_func>
        bool lookup_update(Key key, slow_func slow_get_page) {
            if (main.finded_in_HT(key)) return true;
            if (in.finded_in_HT(key)) return true;

            HashIt key_HT_Iter = out.data_.find(key);
            if (key_HT_Iter != out.data_.end()) {

                if (main.full()) {
                    main.data_.erase(main.queue_.back().first);
                    main.queue_.pop_back();
                }

                main.queue_.splice(main.queue_.begin(), out.queue_, key_HT_Iter->second);
                out.data_.erase(key_HT_Iter);
                main.data_[key] = main.queue_.begin();

                if (DEBUG) dump();
                return true;
            }

            if (in.full()) {

                if (out.full()) {
                    out.data_.erase(out.queue_.back().first);
                    out.queue_.pop_back();
                }

                Key moved_key = in.queue_.back().first;
                in.data_.erase(moved_key);
                out.queue_.splice(out.queue_.begin(), in.queue_, std::prev(in.queue_.end()));
                out.data_[moved_key] = out.queue_.begin();
            }

            in.queue_.push_front(std::make_pair(key, slow_get_page(key)));
            in.data_[key] = in.queue_.begin();

            if (DEBUG) dump();
            return false;

        }

        void dump() const {
            printf("main");
            main.dump();

            printf("in");
            in.dump();

            printf("out");
            out.dump();
            printf("\n");
        }
    };

}