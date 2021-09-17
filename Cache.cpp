#include <iostream>
#include <math.h>
#include "Q2.h"
#include <vector>

#define KEY size_t
#define VALUE int

VALUE slow_get_page_int(KEY key) { return key * key * key; }

int main() {
    std::cout << "we enter\n";

    using namespace q2_cache;

    size_t hit = 0;

    size_t capacity, quantity;

    printf("Enter capacity, then quantity, then numbers\n");
    std::cin >> capacity >> quantity;
    int tmp = 0;

    Q2Cache<VALUE> our_cache(capacity);

    for (int i = 0; i < quantity; i++) {
        std::cin >> tmp;
        if (our_cache.lookup_update(tmp, slow_get_page_int)) hit++;
    }

    printf("Hits - %llu\n", hit);

    return 0;
}










































/*
#include <cassert>
#include <iostream>

#include "LRU.hpp"

int slow_get_page_int(int key) { return key; }
*/
/*
int main() {
  int hits = 0;
  int n;
  size_t m;

  std::cin >> m >> n;
  assert(std::cin.good());
  caches::cache_t<int> c{m};

  for (int i = 0; i < n; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());
    if (c.lookup_update(q, slow_get_page_int))
      hits += 1;
  }

  std::cout << hits << std::endl;
}*/
/*
int main() {
    using namespace LRU_cache;
    LRU<int> lru(50);

    for (int i = 0; i < 10; i++) lru.lookup_update(5, slow_get_page_int);

    return 0;
}*/