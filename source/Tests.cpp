
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <iterator>

#include "Q2.h"

//namespace q2_test {

    struct page {
        int value_;
        size_t key_;
        size_t distance_;

        page(int value, size_t key, size_t distance) : value_(value), key_(key), distance_(distance) {} //?
    };

    struct IdealCache { // ?? Why not class
        using ListIt = std::list<page>::iterator;
        using HashIt = std::unordered_map<size_t, ListIt>::iterator;
        using VectIt = std::vector<size_t>::iterator;

        std::list<page> queue_;
        std::unordered_map<size_t, ListIt> data_;
        size_t size_;
        std::vector<size_t> potok_;
        size_t potok_pointer_;

        IdealCache& operator= (const IdealCache& cache_tmp) = delete;
        IdealCache(const IdealCache&) = delete;

        bool full() const { return (queue_.size() == size_); }

    public:

        IdealCache(size_t size, std::vector<size_t>& potok) : size_(size) {
            printf("In ideal constructor\n");
            potok_ = potok;
            potok_pointer_ = 0;
        }

        bool lookup_update(size_t key, int (*slow_get_page_int) (size_t)) {

            for (int i = 0; i < potok_.size(); i++) std::cout << potok_[i] << " ";
            std::cout << "\n" << potok_pointer_ << " - " << potok_[potok_pointer_] << "\n";

            HashIt key_HT_Iter = data_.find(key);
            if (key_HT_Iter != data_.end()) {
                for (ListIt i = queue_.begin(); i != queue_.end(); i++) --((*i).distance_);

                ++potok_pointer_;
                return true;
            }

            if (!full()) {
                for (ListIt i = queue_.begin(); i != queue_.end(); i++) --((*i).distance_);

                auto find_in_potok_ = potok_pointer_ + 1;               // ????

                while (potok_[potok_pointer_] != key) {//?
                    if (find_in_potok_ == potok_.size()/*potok_.end()*/) return false; //?
                    ++find_in_potok_;
                }

                page new_p(key, slow_get_page_int(key), find_in_potok_ - potok_pointer_);

                ++potok_pointer_;
                return false;
            }

            std::pair<size_t, size_t> erase_candidat = std::make_pair(0, 0); //first is key, second is distance

            for (ListIt i = queue_.begin(); i != queue_.end(); i++) {
                --((*i).distance_);

                if (erase_candidat.second > (*i).distance_) continue;
                erase_candidat.first = (*i).key_;
                erase_candidat.second = (*i).distance_;
            }

            queue_.erase(data_.find(erase_candidat.first)->second);
            data_.erase(erase_candidat.first);

            ++potok_pointer_;
            return false;
        }
    };

    bool intUnitTest(std::string& input, std::string& answer, int (*slow_get_page_int) (size_t)) { // Close file descriptor?

        printf("Into test\n");
        std::ifstream input_potok(input);

        if (input_potok.is_open()) {

            size_t capacity, quantity;
            input_potok >> capacity >> quantity;

            std::vector<size_t> vector_potok;
            for (size_t i = 0; i < quantity; i++)
                input_potok >> vector_potok[i];


            size_t hit = 0;
            printf("Before IdealCache\n");
            IdealCache ideal(capacity, vector_potok);
            printf("Constructor true\n");
            for (size_t i = 0; i < quantity; i++)
                if (ideal.lookup_update(vector_potok[i], slow_get_page_int)) hit++;

            std::cout << "Ideal cache: " << hit << " hits\n";

            q2_cache::Q2Cache<int> cache(capacity);
            for (size_t i = 0; i < quantity; i++)
                if (cache.lookup_update(vector_potok[i], slow_get_page_int)) hit++;

            std::ifstream answer_potok(answer);

            if (answer_potok.is_open()) {
                size_t answer_hit;
                answer_potok >> answer_hit;

                if (answer_hit == hit) {
                    std::cout << "Test from file [" << input << "] work correctly\n";
                    return true;
                }
                else {
                    std::cout << "Program hits - " << hit << ", true hit - " << answer_hit << ". Test from file [" << input << "] failed";
                    return false;
                }
            }

            std::cout << "Your answer file [" << answer << "] wasn't open.";
            std::cout << "In this test was " << hit << " hits. Test failed\n";
            return false;
        }

        std::cout << "File [" << input << "] can't be open. Test failed\n";
        return false;
    }

//}