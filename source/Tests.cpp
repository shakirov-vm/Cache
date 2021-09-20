

#pragma once

#include <fstream>
#include <iostream>

#include "Q2.h"

bool intUnitTest(std::string& input, std::string& answer, int (*slow_get_page_int) (size_t)) {

    std::ifstream input_potok(input);

    if (input_potok.is_open()) {

        size_t capacity, quantity;
        input_potok >> capacity >> quantity;

        size_t hit = 0;

        q2_cache::Q2Cache<int> cache(capacity);
        int tmp = 0;

        for (size_t i = 0; i < quantity; i++) {
            input_potok >> tmp;
            if (cache.lookup_update(tmp, slow_get_page_int)) hit++;
        }

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