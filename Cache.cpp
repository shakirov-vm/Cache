#include <iostream>
#include <math.h>
#include "Q2.h"

int slow_get_page_int(int key) { return key; }

int main() {
    std::cout << "we enter\n";

    using namespace q2_cache;

    Q2Cache<int> our_cache(20);

    for (int i = 0; i < 1; i++) {
        our_cache.lookup_update(static_cast<size_t>(5 * i / sin(i * i)), slow_get_page_int);
    }

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