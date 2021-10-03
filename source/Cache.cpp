#include <iostream>
#include <math.h>
#include <vector>

#include "Q2.h"
#include "Test.h"

using KEY = size_t;
using VALUE = int;

VALUE slow_get_page_int(KEY key) { return key * key * key; }

int main(int argc, char** argv) {
    printf("Where?\n");
    printf("Where?_2\n");
    if (argc == 3) { // There tests
        printf("There??\n");
        std::string input = std::string(argv[1]);
        std::string answer = std::string(argv[2]);

        printf("Before Test\n");
        if (/*q2_test::*/intUnitTest(input, answer, slow_get_page_int)) return 0;

        return -1;
    }
    else if (argc != 3 && argc != 1) {
        std::cout << "There need 1 or 3 args for this programm. You enter " << argc << std::endl;
    }
    else {

        size_t hit = 0;

        size_t capacity, quantity;

        std::cin >> capacity >> quantity;
        int tmp = 0;

        q2_cache::Q2Cache<VALUE> our_cache(capacity);

        for (size_t i = 0; i < quantity; i++) {
            std::cin >> tmp;
            if (our_cache.lookup_update(tmp, slow_get_page_int)) hit++;
        }

        std::cout << hit << "\n";

        return 0;
    }
}
