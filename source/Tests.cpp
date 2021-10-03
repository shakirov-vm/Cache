
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <iterator>

#include "Q2.h"
#include "Test.h"

namespace q2_test {
    
    bool intUnitTest(std::string input, std::string answer, int (*slow_get_page_int) (size_t)) { 

        std::ifstream input_potok(input);

        if (input_potok.is_open()) {

            size_t capacity, quantity;
            input_potok >> capacity >> quantity;

            std::vector<int> vector_potok;

            size_t hit = 0;
            int tmp = 0;

            for (size_t i = 0; i < quantity; i++) {
                input_potok >> tmp;
                vector_potok.push_back(tmp);
            }

            size_t ideal = ideal_hits(vector_potok, slow_get_page_int, capacity);
            std::cout << "Ideal cache: " << ideal << " hits\n";

            q2_cache::Q2Cache<int> cache(capacity);
            for (size_t i = 0; i < quantity; i++)
                if (cache.lookup_update(vector_potok[i], slow_get_page_int)) hit++;

            std::ifstream answer_potok(answer);

            if (answer_potok.is_open()) {
                size_t answer_hit;
                answer_potok >> answer_hit;

                if (answer_hit == hit) {
                    std::cout << "Test from file [" << input << "] work correctly\n";
                    input_potok.close();
                    answer_potok.close();
                    return true;
                }
                else {
                    std::cout << "Program hits - " << hit << ", true hit - " << answer_hit << ". Test from file [" << input << "] failed";
                    input_potok.close();
                    answer_potok.close();
                    return false;
                }
            }

            std::cout << "Your answer file [" << answer << "] wasn't open.";
            std::cout << "In this test was " << hit << " hits. Test failed\n";
            input_potok.close();
            return false;
        }

        std::cout << "File [" << input << "] can't be open. Test failed\n";
        return false;
    }

    size_t ideal_hits(std::vector<int>& vector_potok, int (*slow_get_page_int) (size_t), size_t capacity) {
        IdealCache ideal(capacity, vector_potok);
        size_t quantity = vector_potok.size();
        size_t hit = 0;

        for (size_t i = 0; i < quantity; i++) {
            if (ideal.lookup_update(vector_potok[i], slow_get_page_int)) hit++;

            ++ideal.potok_pointer_;
            if (DEBUG) ideal.dump();
        }
        
        return hit;
    }

    void IdealCache::dump() {
        std::cout << "In Cache[" << queue_.size() << "] ";
        for (ListIt i = queue_.begin(); i != queue_.end(); i++) std::cout << "{" << i->value_ << " - " << i->distance_ << "} -> ";
        printf("List: begin - %d, end - %d\n", queue_.begin()->value_, std::prev(queue_.end())->value_);
        std::cout << "\n";
    }

    bool IdealCache::lookup_update(size_t key, int (*slow_get_page_int) (size_t)) {

        HashIt key_HT_Iter = data_.find(key);
        if (key_HT_Iter != data_.end()) {
            for (ListIt i = queue_.begin(); i != queue_.end(); i++) --(i->distance_);      
            return true;
        }

        size_t find_in_potok_ = potok_pointer_ + 1; 

        while (potok_[find_in_potok_] != key) {
            if (find_in_potok_ == potok_size_) return false; 
            
            ++find_in_potok_;
        }


        if (full()) {
            std::pair<size_t, size_t> erase_candidat = std::make_pair(key, find_in_potok_ - potok_pointer_); //first is key, second is distance
            ListIt list_candidate = queue_.end();

            for (ListIt i = queue_.begin(); i != queue_.end(); i++) {
                --((*i).distance_);

                if (erase_candidat.second > i->distance_) continue;
                erase_candidat.first = i->key_;
                erase_candidat.second = i->distance_;
                list_candidate = i;
            }
            if (list_candidate == queue_.end()) return false;

            queue_.erase(list_candidate);
            data_.erase(erase_candidat.first);
        }

        page new_p(slow_get_page_int(key), key, find_in_potok_ - potok_pointer_);
        queue_.push_back(new_p);
        data_[key] = queue_.end();
       
        return false;
    }

}